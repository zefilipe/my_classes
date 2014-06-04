<?php

/**
 * Html for Analyze Links and Generate PDF.
 * 
 * This script, contains a class with methods, to output html for Analyze Links and Generate PDF.
 * 
 * @author José Filipe Lopes Santos <jfilipe@med.up.pt>
 * @version 1.0 - Date last modified: 01-04-2008
 * @package algpdf
 * @subpackage libraries
 */

// ensure this file is being included by a parent file
defined("_VALID_MOS") or die("Direct Access to this location is not allowed.");


/**
 * Html for Analyze Links and Generate PDF.
 *
 * This class, has methods to output html for Analyze Links and Generate PDF.
 */
class html_algpdf extends html_algpdf_common {
	
	/**
	 * Option (component selected).
	 * @var string
	 */
	var $option = null;
	
	/**
	 * Step in the process of the algpdf.
	 * @var integer
	 */
	var $step = null;
	
	/**
	 * Id of the content.
	 * @var integer
	 */
	var $id_content = null;
	
	/**
	 * Array with the results.
	 * @var array
	 */
	var $rows = array();
	
	/**
	 * Handler of the database.
	 * @var array
	 */
	var $db = array();	
	
	/**
	 * Method Constructor.
	 *
	 * @param string $option option (component selected)
	 * @param integer $step step in the process of the algpdf
	 * @param integer[optional] $id_content id of the content
	 * @param array[optional] $rows array with the results
	 * @param array[optional] $db handler of the database
	 * @return html_algpdf
	 */
	function html_algpdf($option,$step,$id_content = null,$rows = array(),$db = array()){
		
		$this->option = $option;
		$this->step = $step;
		$this->id_content = $id_content;
		$this->rows = $rows;
		$this->db = $db;
	}
	
	
	/**
	 * Output html to display form to select a content item.
	 *
	 * @param string $str_cats string with ids of the categories
	 * @param integer $id_content id of the content
 	 * @param string $name_pdf name of the pdf
 	 * @param integer $overwrite_pdf flag that indicates if overwrite pdf
	 * @param string $submited flag that indicates if has submited the form
 	 * @param string $pdf_ok flag that indicates if the name of the pdf is ok
 	 * @param string $file_pdf file of the pdf (full path)
	 */
	function SelectContent($str_cats,$id_content,$name_pdf,$overwrite_pdf,$submited,$pdf_ok,$file_pdf){
		
		// create a new instance of this class
		$html = new html_algpdf($this->option,$this->step,$this->id_content,$this->rows,$this->db);
		
		// create a new instance of patTemplate class
		$t =& $html->createTemplate();
		
		// read templates from the file
		$t->readTemplatesFromFile("form_select_content.html");
		
		// add variables
		$t->addVars("page",array("js_url" => JAVASCRIPT_URL,"name_pdf_value" => $name_pdf));
		
		// write options of the contents 
		$html->write_contents_options(&$t,"options_content",$id_content,$str_cats);
		
		// write options for overwrite pdf
		$html->write_yesno_options(&$t,"options_overwrite",$overwrite_pdf);
		
		// hiddens
		$hiddens = array("option" => $this->option, "task" => "", "submited" => $submited, "pdf_ok" => $pdf_ok, "step" => $this->step+1, "act" => "analyze_links");
		foreach ($hiddens as $name=>$value){
			$t->addVars("hiddens",array("hidden_name" => $name, "hidden_value" => $value));
			$t->parseTemplate("hiddens","a");
		}
						
		
		///// write note or set ok ////
		
		// if want overwrite, submited any time and is not ok then write note or set ok
		if ($overwrite_pdf == 0 && ereg("y",$submited) && ereg("n",$pdf_ok)){
			
			if (file_exists($file_pdf)){ // pdf already exists
				
				// write note
				$t->addVars("note_or_setok",array("type" => "note",
												  "msg" => "File <b>$file_pdf</b> already exists !. Please, try other name of the pdf"));
				$t->parseTemplate("note_or_setok","a");
				
			} else { // pdf not exists
				
				// set ok
				$t->addVars("note_or_setok",array("type" => "setok"));
				$t->parseTemplate("note_or_setok","a");
			}
		}		
		
		// display output
		$t->displayParsedTemplate();
	}
	
	
	/**
	 * Output html to dislay page for analyzing links.
	 * 
	 * @param string $name_pdf name of the pdf
	 */
	function AnalyzingLinks($name_pdf){
		
		// create a new instance of this class
		$html = new html_algpdf($this->option,$this->step,$this->id_content);
		
		// create a new instance of patTemplate class
		$t =& $html->createTemplate();
		
		// read templates from the file
		$t->readTemplatesFromFile("analyzing_links.html");
		
		// add variables
		$t->addVars("page",array("js_url" => JAVASCRIPT_URL, "images_url" => IMAGES_URL));
		
		// hiddens
		$hiddens = array("option" => $this->option, "task" => "", "step" => $this->step+1, "name_pdf" => $name_pdf, "id_content" => $this->id_content, "act" => "analyze_links");
		foreach ($hiddens as $name=>$value){
			$t->addVars("hiddens",array("hidden_name" => $name, "hidden_value" => $value));
			$t->parseTemplate("hiddens","a");
		}		
		
		// display output
		$t->displayParsedTemplate();
	}
	
	
	/**
	 * Output html to display a list of the links (attachements and contents).
	 *
	 * @param array $attachs array with attachements information
	 * @param array $contents array with contents information
	 * @param string $name_pdf name of the pdf
	 */
	function ListLinks($attachs,$contents,$name_pdf){
		
		// create a new instance of this class
		$html = new html_algpdf($this->option,$this->step,$this->id_content);
		
		// create a new instance for patTemplate class
		$t =& $html->createTemplate();
		
		// read templates from the file
		$t->readTemplatesFromFile("listing_links.html");
		
		// add variables
		$t->addVars("page",array("js_url" => JAVASCRIPT_URL,
								 "n_rows_attachs" => count($attachs),
								 "n_rows_contents" => count($contents)));

								 
		foreach ($attachs as $index=>$attach){ // for each attach
			
			// determine variables
			$index_class = $index % 2;
			$class_attachs = "row$index_class";
			
			// clear variables
			$t->clearVars("row_attach");
			
			// add variables
			$t->addVars("row_attach",array("class_attachs_value" => $class_attachs,
										   "pos_attach" => $index,
										   "id_attach_value" => $attach->id,
										   "filename" => $attach->filename,
										   "content" => $attach->name_content));
										   
			// append to templates
			$t->parseTemplate("row_attach","a");
		}
		
		
		foreach ($contents as $index=>$content){ // for each content
			
			// determine variables
			$index_class = $index % 2;
			$class_contents = "row$index_class";
			
			// clear variables
			$t->clearVars("row_content");
			
			// add variables
			$t->addVars("row_content",array("class_contents_value" => $class_contents,
											"pos_content" => $index,
											"id_content_value" => $content->id,
											"title" => $content->title,
											"category" => $content->title_cat));
											
			// append to templates
			$t->parseTemplate("row_content","a");			
		}
		
		
		// hiddens
		$hiddens = array("option" => $this->option, "task" => "", "step" => $this->step+1, "name_pdf" => $name_pdf, "id_content" => $this->id_content, "boxchecked_attachs" => "0", "boxchecked_contents" => "0", "act" => "analyze_links");
		foreach ($hiddens as $name=>$value){
			$t->addVars("hiddens",array("hidden_name" => $name, "hidden_value" => $value));
			$t->parseTemplate("hiddens","a");
		}		
				
		// display output
		$t->displayParsedTemplate();
	}
	
	
	/**
	 * Output html to display page for generating pdf.
	 */
	function generatingPdf() {
		
		// create a new instance of this class
		$html = new html_algpdf($this->option,4,$this->id_content);
		
		// create a new instance of patTemplate class
		$t =& $html->createTemplate();
		
		// read templates from the file
		$t->readTemplatesFromFile("generating_pdf.html");
		
		// determine location to redirect
		$new_location = $_SERVER['PHP_SELF']."?option=".$this->option."&act=list";
		
		// add variables
		$t->addVars("page",array("js_url" => JAVASCRIPT_URL, "images_url" => IMAGES_URL, "location" => $new_location));
		
		// hiddens
		$hiddens = array("option" => $this->option, "task" => "", "id_content" => $this->id_content, "act" => "analyze_links");
		foreach ($hiddens as $name=>$value){
			$t->addVars("hiddens",array("hidden_name" => $name, "hidden_value" => $value));
			$t->parseTemplate("hiddens","a");
		}		
		
		// display output
		$t->displayParsedTemplate();		
	}	
}

?>