<?php

/**
 * Backend Interface.
 * 
 * @author José Filipe Lopes Santos <jfilipe@med.up.pt>
 * @version 1.0 - Date last modified: 11-03-2008
 * @package algpdf
 * @subpackage main
 */

// ensure this file is being included by a parent file
defined("_VALID_MOS") or die("Direct Access to this location is not allowed.");

// ensure user has access to this function
if (!($acl->acl_check("administration","edit","users",$my->usertype,"components","all")
    | $acl->acl_check("administration","edit","users",$my->usertype,"components","com_algpdf")))
    
	mosRedirect("index2.php",_NOT_AUTH);

	
// includes
require_once("config.algpdf.php");
require_once("lib/functions.inc");
require_once("lib/db_algpdf_conf.inc");
require_once("lib/db_algpdf_list.inc");
require_once("lib/db_algpdf.inc");
require_once("lib/html_algpdf_common.inc");
require_once("lib/html_algpdf_conf.inc");
require_once("lib/html_algpdf_list.inc");
require_once("lib/html_algpdf.inc");
require_once("lib/pdf_utils.inc");


//////// get values of the variables //////

$ids = mosGetParam($_REQUEST,"cid",array());
$step = mosGetParam($_REQUEST,"step",1); // by default, step is 1
$id_content = mosGetParam($_REQUEST,"id_content");
$name_pdf = mosGetParam($_REQUEST,"name_pdf","");
$overwrite_pdf = mosGetParam($_REQUEST,"overwrite_pdf",0);
$pdf_ok = mosGetParam($_REQUEST,"pdf_ok","n");
$submited = mosGetParam($_REQUEST,"submited","n");
$ids_attachs = mosGetParam($_REQUEST,"ids_attachs",array());
$ids_contents = mosGetParam($_REQUEST,"ids_contents",array());


/* ----------------------- oOo ----------------------
	  Executes a function, when selected a option
   ----------------------- oOo ---------------------- */

// executes a function, for this task and action or activitie (act)
switch ($act){

	case "list":
		switch ($task){
			case "remove":
				RemovePdfs($option,$ids);
				break;
				
			default:
				ListPdfs($option);
		}		
		break;
		
	case "conf":
		switch ($task){
			case "save":
				SaveConfiguration($option);
				break;
				
			case "cancel":
				mosRedirect("index2.php?option=$option");
				break;
				
			default:
				ListConfiguration($option);
		}
		break;
		
	default:
		switch ($step){
			case 1:
				SelectContent($option,$step,$id_content,$name_pdf,$overwrite_pdf,$submited,$pdf_ok);
				break;
				
			case 2:
				AnalyzeLinks($option,$step,$id_content,$name_pdf);
				break;
				
			case 3:
				ListLinks($option,$step,$id_content,$name_pdf);
				break;
				
			case 4:
				genPDF($option,$id_content,$name_pdf,$ids_attachs,$ids_contents);
				break;
											
			default:
				echo "step <b>$step</b> is invalid !<br>";	
		}
}

?>