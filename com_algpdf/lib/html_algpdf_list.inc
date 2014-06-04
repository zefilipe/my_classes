<?php

/**
 * Html for Listing of the generated pdfs.
 * 
 * This script, contains a class with methods, to output html for listing of the generated pdfs.
 * 
 * @author José Filipe Lopes Santos <jfilipe@med.up.pt>
 * @version 1.0 - Date last modified: 26-03-2008
 * @package algpdf
 * @subpackage libraries
 */

// ensure this file is being included by a parent file
defined("_VALID_MOS") or die("Direct Access to this location is not allowed.");


/**
 * Html for listing of the generated pdfs.
 *
 * This class, has methods to output html for listing of the generated pdfs.
 */
class html_algpdf_list extends html_algpdf_common {
	
	/**
	 * Option (component selected).
	 * @var string
	 */
	var $option = "";
	
	/**
	 * Array with the results.
	 * @var array
	 */
	var $rows = array();

	/**
	 * Path to files.
	 * @var string
	 */
	var $path = "";
	
	/**
	 * Method constructor.
	 *
	 * @param string $option option (component selected)
	 * @param array[optional] $rows array with the results
	 * @param string[optional] $path path to files
	 * @return html_algpdf_list
	 */
	function html_algpdf_list($option, $rows = array(), $path = ""){
		
		$this->option = $option;
		$this->rows = $rows;
		$this->path = $path;
	}
	
	/**
	 * Output htnl to list all files.
	 */
	function ListFiles(){

		// creates a new patTemplate instance
		$t =& html_algpdf_list::createTemplate();
		
		// reads templates from file
		$t->readTemplatesFromFile("listing_pdfs.html");

		// add variables
		$t->addVars("page",array("js_url" => JAVASCRIPT_URL,
								 "n_rows" => count($this->rows)));
		
		foreach ($this->rows as $index=>$row){ // foreach row
			
			// determine variables
			$index_class = $index % 2; // index of the class, alternates every two rows
			$class = "row".$index_class;
			$path = $this->path;
			// drop last '/' in path, if it exists
			if ($path[strlen($path)-1] == "/")
				$path = substr($path,0,strlen($path)-1);
			
			// clear all old variables values
			$t->clearVars("row");
				
			// add variables
			$t->addVars("row",array("class_name" => $class,
									"pos" => $index,
									"id_value" => $row->id,
									"path_files" => $path,
									"file_name" => $row->file_name,
									"file_size" => $row->file_size,
									"create_time" => mosFormatDate($row->file_created,"%d-%b-%Y %H:%M:%S",0),
									"title_content" => $row->title_content));
									
			// append template
			$t->parseTemplate("row","a");			
		}
	
		// hiddens
		$hiddens = array("option" => $this->option, "task" => "", "boxchecked" => "0", "act" => "list");
		foreach ($hiddens as $name=>$value){
			$t->addVars("hiddens",array("hidden_name" => $name, "hidden_value" => $value));
			$t->parseTemplate("hiddens","a");
		}
				
		// show output
		$t->displayParsedTemplate();
	}	
}

?>