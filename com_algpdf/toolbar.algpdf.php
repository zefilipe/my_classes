<?php

/**
 * Backend Toolbar.
 * 
 * @author José Filipe Lopes Santos <jfilipe@med.up.pt>
 * @version 1.0 - Date last modified: 11-03-2008
 * @package algpdf
 * @subpackage toolbar
 */

// ensure this file is being included by a parent file
defined("_VALID_MOS") or die("Direct Access to this location is not allowed.");

// includes
require_once($mainframe->getPath("toolbar_html"));


/* ----------------------- oOo ----------------------
	  		Show the toolbar for this option
   ----------------------- oOo ---------------------- */

switch ($act){
	
	case "list":
		// toolbar to listing files generated
		toolbar_algpdf::listing_toolbar();
		break;
		
	case "conf":
		// toolbar to configuration
		toolbar_algpdf::configuration_toolbar();
		break;
		
	default:
		// toolbar to analyze links and generate pdf (algpdf)
		toolbar_algpdf::default_toolbar();
}


?>