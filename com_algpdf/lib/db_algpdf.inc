<?php

/**
 * Database algpdf.
 * 
 * This script, contains a class with methods, to manipulate data in database for
 * Analyze Links and Generate PDF (ALGPDF).
 * 
 * @author José Filipe Lopes Santos <jfilipe@med.up.pt>
 * @version 1.0 - Date last modified: 02-04-2008
 * @package algpdf
 * @subpackage libraries
 */

// ensure this file is being included by a parent file
defined("_VALID_MOS") or die("Direct Access to this location is not allowed.");


/**
 * Database algpdf.
 *
 * This class, has methods, to manipulate data in database for
 * Analyze Links and Generate PDF (ALGPDF).
 */
class db_algpdf {
	
	/**
	 * Handler of the database.
	 * @var array
	 */
	var $db = array();
	
	/**
	 * Flag that indicate if analyze links is recursive.
	 * @var integer
	 */
	var $recursive = 0;
	
	/**
	 * Array with links of the attachements.
	 * @var array
	 */
	var $links_attachs = array();
	
	/**
	 * Counters of the attachements.
	 * @var array
	 */	
	var $n_attachs = array();
	
	/**
	 * Array with links of the contents.
	 * @var array
	 */
	var $links_contents = array();
	
	/**
	 * Counters of the contents.
	 * @var integer
	 */
	var $n_contents = array();
	
	/**
	 * Method constructor
	 *
	 * @param array $db handler of the database
	 * @param integer[optional] $recursive flag that indicate if analyze link is recursive
	 * @return db_algpdf
	 */
	function db_algpdf($db,$recursive = 0){
		
		$this->db = $db;
		$this->recursive = $recursive;
	}
	
	
	/**
	 * Analyze links.
	 * 
	 * This method, analyze all the links, separate what is attach to what is content,
	 * and store information of these links in arrays.
	 *
	 * @param integer $id_content id of the content
	 */
	function AnalyzeLinks($id_content){
		
		//******* extract data of this content ********
		
		// create a new instance of this class
		$db_algpdf = new db_algpdf($this->db,$this->recursive);
		
		// extract data of this content
		$row = $db_algpdf->getContentInfo($id_content);
		
		echo "Analyzing links of the content <b>".$row->title."</b> ...<br>";
		echo "Parsing links ...<br>";

		
		//************ Parsing links ***********
		
		// regular expression to match links
		$regexp = "<a\b[^>]*href=\"(.*)\">(.*)<\/a>";
		
		// get all matches of links in the text
		if (preg_match_all("/".$regexp."/siU",$row->text,$matches)){
			
			// only interests links
			$links = $matches[1];
			$titles = $matches[2];
		
			foreach ($links as $index=>$link){ // for each link
				
				if (ereg("\/",$link) && !ereg("(http|www\.)",$link)){ // attachement	
							
					// initialize counters of attachements
					if (empty($this->n_attachs[$id_content])) $this->n_attachs[$id_content]=0;
						
					// add a slash to begin of the attach if does not exist
					if ($link[0] != "/") $link = "/".$link;
					
					// cut all text after the attachement
					$link = ereg_replace("([^\"]*)(\"(.*))","\\1",$link);
					
					// store link details in the array
					$this->links_attachs[$id_content][$this->n_attachs[$id_content]++] = $link;				
					
					// output
					echo "Link: attach <b>$link</b><br>";
					
				} elseif (ereg(";id=",$link)){ // content
								
					// match only the id
					if (ereg("(;id=)([0-9]+)",$link,$regs)){ 
						
						// set variables
						$id = $regs[2];
						$title = strip_tags($titles[$index]);
										
						// get the id of the category and title, of this content
						$sql = "select title,catid from #__content where id=$id";
						$this->db->setQuery($sql);
						$row2 = $this->db->loadObjectList();
						
						if ($this->db->getErrorNum()){
							echo $this->db->getErrorMsg();
							return ;
						}
						$row2 = $row2[0];

						
						// initialize counters of contents
						if (empty($this->n_contents[$row2->catid])) $this->n_contents[$row2->catid]=0;
																
						// store link details in the array
						$this->links_contents[$row2->catid][$this->n_contents[$row2->catid]]["id"] = $id;
						$this->links_contents[$row2->catid][$this->n_contents[$row2->catid]++]["title"] = $row2->title;
						
						// output
						echo "Link: content  title is <b>".$row2->title."</b> and id is <b>$id</b><br>";
						
						// if is recursive
						if ($this->recursive == 1)
							// call this function recursively, for each id
							db_algpdf::AnalyzeLinks($id);
					}
				}				
			}			
		}			
	}
	
	
	/**
	 * Get categories.
	 * 
	 * This method, get categories data.
	 *
	 * @return array $cats array with the categories
	 */
	function getCategories(){
		
		$cats = array(); // array with categories, with entries like 'id_categorie' => 'name_categorie'

		// extract data of the categories
		// only interest thats is published, and with numeric section (exclude 'com_*')
		$sql = "select id,title from #__categories ";
		$sql .= "where published=1 and section regexp '^[[:digit:]]$' = 1 ";
		$sql .= "order by title";
		
		$this->db->setQuery($sql);
		$rows = $this->db->loadObjectList();
		
		if ($this->db->getErrorNum()){
			echo $this->db->getErrorMsg();
			return $cats;
		}
		
		foreach ($rows as $index=>$row) // for each row
			// store info to array
			$cats[$row->id]	 = $row->title;
		
		return $cats;
	}
	
	
	/**
	 * Save the information of the links.
	 * 
	 * This method, save the information of the links analyzed.
	 *
	 * @param array $cats array with the categories
	 */
	function saveLinksInfo($cats){
		
		////// remove all entries if exist, in the temporary table /////
		$delete = "delete from #__algpdf_tmp";
		$this->db->setQuery($delete);
		
		if (!$this->db->query()){
			echo $this->db->getErrorMsg();
			return ;
		}
		
		
		///// insert new entries //////
		
		// query to insert rows
		$insert_query = "insert into #__algpdf_tmp (id_type,id_content,title,id_cat,title_cat,filename) ";

		// attachements
		foreach ($this->links_attachs as $id_content=>$attachs){  // for each content
			foreach ($attachs as $index=>$attach){ // for each attach

				// insert data of this attachement
				$insert = $insert_query."values(2,$id_content,null,null,null,'$attach')";
				$this->db->setQuery($insert);
				
				if (!$this->db->query()){
					echo $this->db->getErrorMsg();
					return ;
				}
			}			
		}
		
		// contents
		foreach ($this->links_contents as $id_cat=>$contents_by_cat){ // for each categorie
			foreach ($contents_by_cat as $index=>$contents){ // for each content
				
				// set variables
				$id = $contents["id"];
				$title = $contents["title"];
				$title_cat = $cats[$id_cat];
				
				// replace any ' to '', otherwise it get an database error
				if (strpos($title,"'"))
					$title = str_replace("'","''",$title);
				
				// insert data of this content
				$insert = $insert_query."values(1,$id,'$title',$id_cat,'$title_cat',null)";
				$this->db->setQuery($insert);
				
				if (!$this->db->query()){
					echo $this->db->getErrorMsg();
					return ;
				}				
			}			
		}		
	}
	
	
	/**
	 * Get links information.
	 * 
	 * This method, get links information.
	 *
	 * @param integer $id_content id of the main content
	 * @param string[optional] $str_ids_attachs string with ids of the attachements
	 * @param string[optional] $str_ids_contents string with ids of the contents 
	 * @return array array(array with attachements, array with contents)
	 */
	function getLinksInfo($id_content,$str_ids_attachs = "",$str_ids_contents = ""){
		
		$attachs = array();
		$contents = array();
		
		// extract data of the attachements in the links
		// id_type=2 (2 - attachements), and excludes attachs in the main content
		$sql = "select t.*,c.title as name_content from #__algpdf_tmp t, #__content c ";
		$sql .= "where t.id_content=c.id and  t.id_type=2 and t.id_content != $id_content ";
		if ($str_ids_attachs != "") $sql .= "and t.id in ($str_ids_attachs) ";
		$sql .= "order by t.filename";
		$this->db->setQuery($sql);
		$attachs = $this->db->loadObjectList();
		
		if ($this->db->getErrorNum()){
			echo $this->db->getErrorMsg();
			return array($attachs,$contents);
		}
		
		// dont found attachements
		if (count($attachs) == 0){
			
			// extract data of the attachements in the links
			// maybe a single content analyzed, and in this case dont exclude this id of the main content
			$sql2 = "select t.*,c.title as name_content from #__algpdf_tmp t, #__content c ";
			$sql2 .= "where t.id_content=c.id and t.id_type=2 ";
			if ($str_ids_attachs != "") $sql2 .= "and t.id in ($str_ids_attachs) ";
			$sql2 .= "order by t.filename";
			$this->db->setQuery($sql2);
			$attachs = $this->db->loadObjectList();
			
			if ($this->db->getErrorNum()){
				echo $this->db->getErrorMsg();
				return array($attachs,$contents);
			}			
		}
		
		// extract data of the contents in the links (id_type= 1 - contents)
		$sql3 = "select * from #__algpdf_tmp ";
		$sql3 .= "where id_type=1 ";
		if ($str_ids_contents != "") $sql3 .= "and id in ($str_ids_contents) ";
		$sql3 .= "order by title";
		$this->db->setQuery($sql3);
		$contents = $this->db->loadObjectList();		
		if ($this->db->getErrorNum()){
			echo $this->db->getErrorMsg();
			return array($attachs,$contents);
		}
		
		return array($attachs,$contents);
	}
	
	
	/**
	 * Get a sugested name of the pdf.
	 * 
	 * This method get a sugested name of the pdf, based in this title.
	 *
	 * @param integer $id_content id of the content
	 * @return string $filename name of the file sugested
	 */
	function getSugestedNameOfPdf($id_content){
		
		$filename = "";
		
		// extract title of the content
		$sql = "select title from #__content where id=$id_content";
		$this->db->setQuery($sql);
		$rows = $this->db->loadObjectList();
		
		if ($this->db->getErrorNum()){
			echo $this->db->getErrorMsg();
			return $filename;
		}
		
		// get only the first row
		$rows = $rows[0];
		
		// get the title value to filename
		$filename = $rows->title;
		
		// trim & to lower
		$filename = strtolower(trim($filename));
		
		// translate charactes with accents
		$filename = strtr($filename,"áéíõùàâêîòúãçóûÀÈÌÁÉÕÙÂÊÎÒÚÃÇÓÛ","aeiouaaeiouacouAEIAEOUAEIOUACOU");
		
		// replace non alphanumeric characters to underscore ('_')
		if (!eregi("^([a-z0-9_]+)$",$filename))
			$filename = ereg_replace("[^a-zA-Z0-9]","_",$filename);
				
		// replace multiple underscore to a single underscore
		if (ereg("[_]{2,}",$filename))
			$filename = ereg_replace("[_]{2,}","_",$filename);
		
		return $filename;		
	}
	
	
	/**
	 * Transform arrays with links information.
	 *
	 * @param array $attachs array with the attachements
	 * @param array $contents array with the contents
	 * @return array array(new array with attachements, new array with contents)
	 */
	function TransformArraysLinksInfo($attachs,$contents){
		
		$new_attachs = array(); // new array with the attachs
		$n_attachs = array(); // counter of the attachs
		$new_contents = array(); // new array with the contents
		$n_contents = array(); // counter of thw contents
		
		foreach ($attachs as $index=>$attach){ // for each attachement
			
			// initialization
			if (empty($n_attachs[$attach->id_content])) $n_attachs[$attach->id_content]=0;
			
			// store info into new array
			$new_attachs[$attach->id_content][$n_attachs[$attach->id_content]++] = $attach->filename;
		}
				
		foreach ($contents as $index=>$content){ // for each content
			
			// initialization
			if (empty($n_contents[$content->id_cat])) $n_contents[$content->id_cat]=0;
			
			// store info into new array
			$new_contents[$content->id_cat][$n_contents[$content->id_cat]]["id"] = $content->id_content;
			$new_contents[$content->id_cat][$n_contents[$content->id_cat]++]["title"] = $content->title;
		}

		return array($new_attachs,$new_contents);	
	}
	
	
	/**
	 * get the content info.
	 * 
	 * This method, get the content info.
	 *
	 * @param integer $id_content id of the content
	 * @return array $row row with the data of these content
	 */
	function getContentInfo($id_content){

		$row = array();
		
		// extract data of this content
		$sql = "select c.title, if(length(c.fulltext)=0,c.introtext,concat(c.introtext,c.fulltext)) as text, c.catid ";
		$sql .= "from #__content c where id=$id_content";
		$this->db->setQuery($sql);
		$row = $this->db->loadObjectList();
		
		if ($this->db->getErrorNum()){
			echo $this->db->getErrorMsg();
			return $row;
		}
		
		// only interests the first row
		$row = $row[0];		
		
		return $row;
	}

	
	/**
	 * Save the pdf information.
	 * 
	 * This method, save the pdf information. 
	 *
	 * @param string $file_name name of the file
	 * @param string $file_size size of the file
	 * @param string $file_created date of the creation of the file
	 * @param integer $id_content id of the content
	 */
	function savePdfInfo($file_name,$file_size,$file_created,$id_content){
		
		// verify if it has any record with this filename
		$sql = "select id from #__algpdf_pdfs where file_name = '$file_name'";
		$this->db->setQuery($sql);
		$rows = $this->db->loadObjectList();
		
		if ($this->db->getErrorNum()){
			echo $this->db->getErrorMsg();
			return ;
		}
		
		$count = 0;
		$id = -1;
		foreach ($rows as $index=>$row){
			$id = $row->id;
			$count++;
		}
		
		if ($count == 0){ // does not exist any record wth this filename
			
			// insert a new record for this pdf file
			$insert = "insert into #__algpdf_pdfs (file_name,file_size,file_created,id_content) ";
			$insert .= "values('$file_name','$file_size',str_to_date('$file_created','%d-%m-%Y %H:%i:%s'),$id_content)";
			$this->db->setQuery($insert);
			
			if (!$this->db->query()){
				echo $this->db->getErrorMsg();
				return ;
			}
						
		} else { // exist any record with this filename
			
			// update record for this pdf file
			$update = "update #__algpdf_pdfs set file_name='$file_name', file_size='$file_size',";
			$update .= "file_created=str_to_date('$file_created','%d-%m-%Y %H:%i:%s'), id_content=$id_content ";
			$update .= "where id=$id";			
			$this->db->setQuery($update);
			
			if (!$this->db->query()){
				echo $this->db->getErrorMsg();
				return ;
			}
		}		
	}
}

?>