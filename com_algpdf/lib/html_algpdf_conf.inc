<?php

/**
 * Html for configuration interface.
 * 
 * This script, contains a class with methods to output html of the configuration interface.
 * 
 * @author José Filipe Lopes Santos <jfilipe@med.up.pt>
 * @version 1.0 - Date last modified: 14-03-2008
 * @package algpdf
 * @subpackage libraries
 */

// ensure this file is being included by a parent file
defined("_VALID_MOS") or die("Direct Access to this location is not allowed.");

/**
 * Html for configuration interface.
 *
 * This class, has methods to output html of the configuration interface.
 */
class html_algpdf_conf extends html_algpdf_common {
	
	/**
	 * Option.
	 *
	 * @var string
	 */
	var $option = "";
	
	/**
	 * Array with the rows of the database.
	 *
	 * @var array
	 */
	var $rows = array();
	
	/**
	 * Handler of the database.
	 * @var array
	 */
	var $db = array();	
	
	/**
	 * Method constructor.
	 *
	 * @param string $option option
	 * @param string $rows rows of the database.
	 * @param array $db handler of the database
	 * @return html_algpdf_conf
	 */
	function html_algpdf_conf($option = "", $rows = array(), $db = array()){
		
		$this->option = $option;
		$this->rows = $rows;
		$this->db = $db;
	}
	
	
	/**
	 * Output html to list configuration.
	 */
	function ListConfiguration(){
		
		// create a new instance of this class
		$html_conf = new html_algpdf_conf($this->option,$this->rows,$this->db);
		
		// create a new patTemplate instance
		$t =& $html_conf->createTemplate();
				
		// read templates from file
		$t->readTemplatesFromFile("form_configuration.html");		
		
		// set variables
		$t->addVars("page",array("js_url" => JAVASCRIPT_URL,
								 "path_storefiles_value" => $this->rows->path_storefiles));
		
		// write options
		$html_conf->write_yesno_options(&$t,"options_recursive",$this->rows->recursive);
		$html_conf->write_yesno_options(&$t,"options_verify_cont",$this->rows->verify_contents);
		$html_conf->write_yesno_options(&$t,"options_verify_attachs",$this->rows->verify_attachs);
		$html_conf->write_yesno_options(&$t,"options_show_cat",$this->rows->show_cat);
		$html_conf->write_categories_options(&$t,"options_cats",$this->rows->analyze_cats);
		
		// hiddens
		$hiddens = array("option" => $this->option, "task" => "", "analyze_cats" => $this->rows->analyze_cats ,"id_conf" => $this->rows->id_conf, "act" => "conf");
		foreach ($hiddens as $name=>$value){
			$t->addVars("hiddens",array("hidden_name" => $name, "hidden_value" => $value));
			$t->parseTemplate("hiddens","a");
		}
		
		// show output
		$t->displayParsedTemplate();
	}
}

?>