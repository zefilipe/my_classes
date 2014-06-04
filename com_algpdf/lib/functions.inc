<?php

/**
 * Common functions.
 * 
 * @author José Filipe Lopes Santos <jfilipe@med.up.pt>
 * @version 1.0 - Date last modified: 29-05-2008
 * @package algpdf
 * @subpackage libraries
 */

// ensure this file is being included by a parent file
defined("_VALID_MOS") or die("Direct Access to this location is not allowed.");

/**
 * List configuration.
 *
 * @param string $option option (component) selected
 */
function ListConfiguration($option){
	global $database;

	$rows = array();
	
	// create a new instance to class database configuration
	$db_conf = new db_algpdf_conf($database);
	
	// get the configuration info
	$rows = $db_conf->getConfigurationInfo();
	
	// create a new instance to class html for configuration interface
	$html_conf = new html_algpdf_conf($option,$rows,$database);
	
	// show output to configuration interface
	$html_conf->ListConfiguration();;
}


/**
 * Save configuration.
 *
 * @param string $option option (component) selected
 */
function SaveConfiguration($option){
	global $database;

	$ok = false;
	
	// create a new instance to class database configuration
	$db_conf = new db_algpdf_conf($database);
	
	// save configuration
	$ok = $db_conf->UpdateConfiguration();
	
	if (!$ok) // failed
		exit();
	else // sucess 
		// redirect and show sucess message
		mosRedirect("index2.php?option=$option&act=conf","Configuration Saved");
}


/**
 * Listing of the generated pdfs.
 *
 * @param string $option option (component) selected
 */
function ListPdfs($option){
	global $database;
	
	$row_conf = array();
	$rows_list = array();
	$path = "";
	
	// create a new instance to class database configuration
	$db_conf = new db_algpdf_conf($database);
	
	// get the configuration info
	$row_conf = $db_conf->getConfigurationInfo();
	
	// gets the path value of the configuration
	$path = $row_conf->path_storefiles;
	
	// drop the application root from the path
	$path = dropAppRootFromPath($path,APPLICATION_NAME);
	
	// create a new instance to class database listing of the generated pdfs
	$db_list = new db_algpdf_list($database);
	
	// gets the data of the generated files
	$rows_list = $db_list->getFilesData();
	
	// create e new instance to class html for listing of the generated pdfs
	$html = new html_algpdf_list($option,$rows_list,$path);
	
	// show output of the listing
	$html->ListFiles();	
}


/**
 * Drops the application root from the path.
 *
 * @param string $full_path full path to files. 
 * @param string $app_name application name
 * @return string $path resulting path
 */
function dropAppRootFromPath($full_path,$app_name){
	
	$path = "";
	
	// if the name of the application is not in path, show error
	if (!ereg($app_name,$full_path)){
		echo "application <b>$application</b> not found in path <b>$full_path</b>";
		return $path;		
	}
	
	// drop application root from the full path
	$path = substr($full_path,strpos($full_path,$app_name)+strlen($app_name),strlen($full_path));
	
	return $path;
}


/**
 * Remove the selected pdfs.
 * 
 * This function, remove the selected pdfs, 
 * from the database and from the filesystem.
 *
 * @param string $option option (component) selected
 * @param string $ids string with ids of the pdfs
 */
function RemovePdfs($option,$ids){
	global $database;
	
	$row_conf = array();
	$rows_list = array();
	$path = "";
	$ok = false;

	// if dont have items selected to delete, show error message and exits
	if (!is_array($ids) || count($ids) < 1){
		echo "<script> alert('Select an item to delete'); window.history.go(-1);</script>n";
		exit();
	}
	
	// create a new instance to class database configuration
	$db_conf = new db_algpdf_conf($database);
	
	// get the configuration info
	$row_conf = $db_conf->getConfigurationInfo();
	
	// get the path in configuration, and remove slash at the end of the path if exists
	$path = $row_conf->path_storefiles;
	if ($path[strlen($path)-1] == "/") $path = substr($path,0,strlen($path)-1);
	
	// create a new instance to class database listing of the generated pdfs
	$db_list = new db_algpdf_list($database);
	
	// get data of the selected pdfs
	$rows_list = $db_list->getFilesData(implode(",",$ids));
	
	// remove data of the selected pdfs
	$ok = $db_list->DeleteFiles($ids);
	
	if (!$ok) // failed
		exit();
		
	else { // sucess
		
		// delete the selected files
		foreach ($rows_list as $index=>$row){ // for each file
			
			// determine full path to file
			$file = $path."/".$row->file_name;
			
			// delete this file
			if (!unlink($file)){
				echo "<script> alert('Cant delete file $file'); window.history.go(-1);</script>n";
				exit();
			}
		}		
	}
	
	mosRedirect("index2.php?option=$option&act=list","PDFs removed");
}


/**
 * Display the form to select a content item.
 *
 * @param string $option option (component) selected
 * @param integer $step current step in the process of the algpdf
 * @param integer $id_content id of the content
 * @param string $name_pdf name of the pdf
 * @param integer $overwrite_pdf flag that indicates if overwrite pdf
 * @param string $submited flag that indicates if has submited the form
 * @param string $pdf_ok flag that indicates if the name of the pdf is ok
 */
function SelectContent($option,$step,$id_content,$name_pdf,$overwrite_pdf,$submited,$pdf_ok){
	global $database;
		
	$row_conf = array();
	$str_cats = "";
	$path = "";
	
	// create a new instance to class database configuration
	$db_conf = new db_algpdf_conf($database);
	
	// get the configuration info
	$row_conf = $db_conf->getConfigurationInfo();

	// get the string of ids of the categories to analyze links, and the path to store files
	$str_cats = $row_conf->analyze_cats;
	$path = $row_conf->path_storefiles;
	
	// determine file of the pdf
	$file_pdf = "";
	if ($path[strlen($path)-1] == "/") $path = substr($path,0,strlen($path)-1);
	$file_pdf = $path."/".$name_pdf.".pdf"; // file = path + name of the pdf + ".pdf"
		
	// create a new instance to class html for analyze links and generate pdf
	$html = new html_algpdf($option,$step,null,array(),$database);
	
	// output html to display form to select content item
	$html->SelectContent($str_cats,$id_content,$name_pdf,$overwrite_pdf,$submited,$pdf_ok,$file_pdf);
}


/**
 * Display the page to analyzing links.
 *
 * @param string $option option (component) selected
 * @param integer $step current step in the process of the algpdf
 * @param integer $id_content id of the main content
 * @param string $name_pdf name of the pdf
 */
function AnalyzeLinks($option,$step,$id_content,$name_pdf){
	global $database;
	
	$row_conf = array();
	$cats = array();
	$recursive = 0;
	
	// create a new instance to class html for analyze links and generate pdf
	$html = new html_algpdf($option,$step,$id_content);
	
	// output html to display analyzing links
	$html->AnalyzingLinks($name_pdf);	

	// create a new instance to class database configuration
	$db_conf = new db_algpdf_conf($database);
	
	// get configuration info
	$row_conf = $db_conf->getConfigurationInfo();
	
	// get the value of the flag recursive, in the configuration
	$recursive = $row_conf->recursive;
	
	// create a new instance to class database algpdf
	$db_algpdf = new db_algpdf($database,$recursive);
	
	// analyzing links
	$db_algpdf->AnalyzeLinks($id_content);

	echo "Extracting categories ...<br>";
	
	// get the categories data
	$cats = $db_algpdf->getCategories();
	
	echo "Saving the links information ....<br>";
	
	// save the links information
	$db_algpdf->saveLinksInfo($cats);
}


/**
 * Listing of the links.
 *
 * @param string $option option (component) selected
 * @param integer $step current step in the process of the algpdf
 * @param integer $id_content id of the main content
 * @param string $name_pdf name of the pdf
 */
function ListLinks($option,$step,$id_content,$name_pdf){
	global $database;
	
	$attachs = array();
	$contents = array();
	
	// creates a new instance to class database algpdf
	$db_algpdf = new db_algpdf($database);
	
	// get links information
	list($attachs,$contents) = $db_algpdf->getLinksInfo($id_content);
	
	// creates a new instance to class html for analyze links and generate pdf
	$html = new html_algpdf($option,$step,$id_content);
	
	// output html to list links
	$html->ListLinks($attachs,$contents,$name_pdf);
}


/**
 * Generate the pdf file.
 *
 * @param string $option option (component) selected
 * @param integer $id_content id of the content
 * @param string $name_pdf name of the pdf
 * @param string $ids_attachs string with ids of the the selected attachements
 * @param string $ids_contents string with ids of the selected contents
 */
function genPDF($option,$id_content,$name_pdf,$ids_attachs,$ids_contents){
	global $database;
	
	// initialize variables and arrays
	$row_conf = array();
	$row_content = array();
	$cats = array();
	$str_ids_attachs = "null";
	$str_ids_contents = "null";
	$attachs = array();
	$contents = array();
	$links_attachs = array();
	$links_contents = array();
	$verify_contents = 0;
	$verify_attachs = 0;
	$show_cat = 0;
	$path = "";
	$error_msg = "";
	$fp = "";
	$title_newsletter = "";
	
	
	//******* Show loading page, and button to continue *********
	
	// creates a new instance to class html for analyze links and generate pdf
	$html = new html_algpdf($option,4,$id_content);

	// output html to display page for genarating pdf
	$html->generatingPdf();

	
	//******** Get configuration info **********
	
	// create a new instance to class database configuration
	$db_conf = new db_algpdf_conf($database);
	
	echo "get the configuration parameters ...<br>";
	
	// get the configuration info
	$row_conf = $db_conf->getConfigurationInfo();
	
	// get values of the configuration settings
	$verify_attachs = $row_conf->verify_attachs;
	$verify_contents = $row_conf->verify_contents;
	$show_cat = $row_conf->show_cat;
	$path = $row_conf->path_storefiles;
		
	
	//***** Get categories and links info *******
	
	// create a new instance to class database alfpdf
	$db_algpdf = new db_algpdf($database,0);
	
	echo "extracting categories ...<br>";
	
	// gets the categories data
	$cats = $db_algpdf->getCategories();
		
	echo "get the links information ...<br>";
	
	// get strings with ids of attachements and contents
	if (count($ids_attachs) > 0) $str_ids_attachs = implode(",",$ids_attachs);
	if (count($ids_contents) > 0) $str_ids_contents = implode(",",$ids_contents);
	
	// get links information
	list($attachs,$contents) = $db_algpdf->getLinksInfo($id_content,$str_ids_attachs,$str_ids_contents);
		
	// transform arrays with links information
	list($links_attachs,$links_contents) = $db_algpdf->TransformArraysLinksInfo($attachs,$contents);

	
	//****** determine file of the pdf ********
	$file_pdf = "";
	// path contains '/' at the end then remove it
	if ($path[strlen($path)-1] == "/") $path = substr($path,0,strlen($path)-1);
	$file_pdf = $path."/".$name_pdf.".pdf"; // file = path + name of the pdf + ".pdf"

	
	//****** write newsletter and related contents to html file *******
		
	// get newsletter info
	$row_content = $db_algpdf->getContentInfo($id_content);
	$title_newsletter = $row_content->title;
	
	// create a new instance to class pdf utilities
	$pdf_utils = new pdf_utils();
	
	echo "append content <b>".$row_content->title."</b> to temporary html file ...<br>";
	
	// open file with newsletter
	$fp = $pdf_utils->open_news_html();
	
	// append the header and the header of the newsletter
	$pdf_utils->append_template($fp,TEMPLATES_PATH."/header.html");
	$pdf_utils->append_template($fp,TEMPLATES_PATH."/header_newsletter.html");
	
	// append html of the content
	$pdf_utils->append_content($fp,$row_content->text);
		
	// append the footer of the newsletter
	$pdf_utils->append_template($fp,TEMPLATES_PATH."/footer_newsletter.html");
	
	// if want analyze contents (configuration) then append contents to html file
	// and have contents selected
	if ($verify_contents == 1 && count($links_contents) > 0){ // want analyze contents and have contents selected

		// append open table of the contents
		$pdf_utils->append_template($fp,TEMPLATES_PATH."/open_table_contents.html");
		
		foreach ($cats as $id_cat=>$title_cat){ // for each categorie
			
			// if dont have contents in this categorie then skip to the next
			if (empty($links_contents[$id_cat])) continue;
			
			// if want show categortie then append categorie info to html file
			if ($show_cat == 1){
				echo "append category <b>$title_cat</b> to temporary html file ...<br>";
				$pdf_utils->append_template($fp,TEMPLATES_PATH."/category.html",array("category" => $title_cat));
			}
			
			foreach ($links_contents[$id_cat] as $index=>$info_contents){ // for each content of this category
				
				// set variables
				$id_content2 = $info_contents["id"];
				$title_content2 = $info_contents["title"];
								
				// get the content info
				$row_content = $db_algpdf->getContentInfo($id_content2);
				
				echo "append content <b>".$row_content->title."</b> to temporary html file ...<br>";				
				
				// append title of the content
				$pdf_utils->append_template($fp,TEMPLATES_PATH."/title_content.html",array("title_content" => $row_content->title));
				
				// append open td of the content
				$pdf_utils->append_template($fp,TEMPLATES_PATH."/open_td_content.html");
				
				// append html of the content
				$pdf_utils->append_content($fp,$row_content->text);				
				
				// append close td of the content
				$pdf_utils->append_template($fp,TEMPLATES_PATH."/close_td_content.html");
			}
		}
		
		// append close table of the contents
		$pdf_utils->append_template($fp,TEMPLATES_PATH."/close_table_contents.html");
	}
			
	// append the footer
	$pdf_utils->append_template($fp,TEMPLATES_PATH."/footer.html");	

	// close file with newsletter
	$pdf_utils->close_news_html($fp);
	
	
	//************ Generate pdf of the html file ***********
	
	echo "generating pdf of the temporary html file ... <br>";
	
	// other includes
	require_once(HTML2PS_PDF_PATH."/config.inc.php");
	require_once(HTML2PS_PDF_PATH."/pipeline.factory.class.php");
	require_once(HTML2PS_PDF_PATH."/fetcher.url.class.php");
	parse_config_file(HTML2PS_PDF_PATH."/html2ps.config");
	
	// convert html file to encoding utf-8
	$html_contents = file_get_contents(FILE_NEWS_HTML_PATH);
	$html_contents = utf8_encode($html_contents);
	file_put_contents(FILE_NEWS_HTML_PATH,$html_contents);
		
	// determine utl of the html file
	$html_url = "http://".$_SERVER["HTTP_HOST"].FILE_NEWS_HTML_URL;
	
	// generate pdf
	$pdf_utils->generatePDF($html_url,$title_newsletter);
	
	// move generated pdf to temporary directory
	copy(FILE_PDF_OUT,FILE_PDF_TMP);
	unlink(FILE_PDF_OUT);

	
	//************ Join attachements to pdf ***********
	
	// if want to verify attachs then join these attachs to pdf
	if ($verify_attachs == 1){
		
		echo "join attachements to pdf ....<br>";
		
		// initialize variables
		$files = array(); // array with files
		$n_files = 0; // counter of the files
		
		// determine array with files to join to pdf
		foreach ($links_attachs as $id_content2=>$info_attach_content){ // for each content
			foreach ($info_attach_content as $index=>$attach){ // for each attach
				
				// store this attach in the array of the files,
				// and appends before the application root path
				$files[$n_files++] = APP_ROOT_PATH.$attach;
			}
		}
		
		// eliminate duplicated files
		$files =  array_unique($files);
					
		// join attachs to pdf
		$pdf_utils->JoinAttachsToPdf(FILE_PDF_COMPLETE_TMP,FILE_PDF_TMP,$files);
	
	} else { // dont want verify attachs
	
		// copy the pdf file to the complete pdf file
		copy(FILE_PDF_TMP,FILE_PDF_COMPLETE_TMP);
	}
	
	
	//********* Copy the temporary complete pdf file to its pdf file to dowload *****
	echo "Copying pdf file to <b>$file_pdf</b> ...<br>";
	copy(FILE_PDF_COMPLETE_TMP,$file_pdf);
	
	
	//******** Collect pdf file properties ******
	echo "Collecting pdf file properties ... <br>";
	$file_name = $name_pdf.".pdf"; // file name
	$file_size = filesize($file_pdf) or die("Cannot determine size of the file <b>$file_pdf</b>");
	$file_size = getFormatedSize($file_size);
	$file_created = date("d-m-Y H:i:s");
	
	//******** Save the pdf information (database) *********
	echo "Saving the pdf information in the database ... <br>";
	$db_algpdf->savePdfInfo($file_name,$file_size,$file_created,$id_content);
	
	//******* Detete temporary files ***********
	echo "Deleting temporary files ... <br>";
	unlink(FILE_NEWS_HTML_PATH);
	unlink(FILE_PDF_TMP);
	unlink(FILE_PDF_COMPLETE_TMP);
}


/**
 * get the formated size.
 *
 * @param integer $size size in bytes
 * @return string $formated_size formated size
 */
function getFormatedSize($size){
	
	$formated_size = "";
	$n = 0; // number of iteractions
	
	while ($size >= 1024){
		$size = $size / 1024;
		$n++;
	}
	
	// format size to contais only one decimal place
	$size = number_format($size,1,".","");
	
	// append byte unit
	if ($n == 1)
		$formated_size = $size." KB";
	elseif ($n == 2)
		$formated_size = $size." MB";
	elseif ($n == 3)
		$formated_size = $size." GB";
	else 
		$formated_size = $size;
		
	return $formated_size;
}

?>