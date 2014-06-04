<?php

/**
 * Database configuration.
 * 
 * This script, contains a class with methods, to manipulate data in database for configuration interface.
 * 
 * @author José Filipe Lopes Santos <jfilipe@med.up.pt>
 * @version 1.0 - Date last modified: 25-03-2008
 * @package algpdf
 * @subpackage libraries
 */

// ensure this file is being included by a parent file
defined("_VALID_MOS") or die("Direct Access to this location is not allowed.");


/**
 * Database configuration.
 *
 * This class, has methods, to manipulate data in database for configuration interface.
 */
class db_algpdf_conf {
	
	/**
	 * Handler of the database.
	 * @var array
	 */
	var $db = array();	
		
	/**
	 * Method constructor.
	 *
	 * @param array $db handler of the database
	 * @return db_algpdf_conf
	 */
	function db_algpdf_conf($db){
		
		$this->db = $db;
	}
	
	
	/**
	 * Get the configuration info.
	 * 
	 * This method, get the configuration info.
	 *
	 * @return array $rows results
	 */
	function getConfigurationInfo(){
		
		$rows = array();

		// extract data to configuration
		$sql = "select * from #__algpdf_conf";
		$this->db->setQuery($sql);
		$rows = $this->db->loadObjectList();
		
    	if ($this->db->getErrorNum()){    		
    		echo $this->db->stderr();
    		return $rows;
    	}
		
    	// the configuration data is in the first row
    	$rows = $rows[0];		
    	
    	return $rows;
	}
	
	
	/**
	 * Update the configuration.
	 * 
	 * This method, update the configuration in the database.
	 *
	 * @return false - if it fails; true - otherwise
	 */
	function UpdateConfiguration(){
						
		// create a configuration table instance
		$row = new db_algpdf_conf_table($this->db);
		
		// bind it to the table (prepare copy form data to one regist in the database)
		if (!$row->bind($_POST)) {		
			echo "<script> alert('".$row->getError()."'); window.history.go(-1);</script>n";
			return false;
		}
	
		// store it in the db
		if (!$row->store()) {
			echo "<script> alert('".$row->getError()."'); window.history.go(-1); </script>n";
			return false;
		}

		return true;	
	}
}


/**
 * Configuration table.
 * 
 * This class, has methods, to manipulate configuration table.
 */
class db_algpdf_conf_table extends mosDBTable {

	/**
	 * Id of the configuration
	 * @var integer
	 */
	var $id_conf = null;
	
	/**
	 * Flag that indicates if the analyze links is recursive.
	 * @var integer
	 */
	var $recursive = null;
	
	/**
	 * Flag that indicates if the contents is to be analyzed.
	 * @var integer
	 */
	var $verify_contents = null;
	
	/**
	 * Flag that indicates if the attachs is to be analyzed.
	 * @var integer
	 */
	var $verify_attachs = null;
	
	/**
	 * Flag that indicates if show categories in pdf.
	 * @var integer
	 */
	var $show_cat = null;
	
	/**
	 * Path to store files generated.
	 * @var string
	 */
	var $path_storefiles = null;
	
	/**
	 * Categories to analyze.
	 * @var string
	 */
	var $analyze_cats = null;
	
	/**
	 * Method constructor.
	 *
	 * @param array $db handler of the database
	 * @return db_algpdf_conf_table
	 */
	function db_algpdf_conf_table(&$db){
		$this->mosDBTable('#__algpdf_conf','id_conf',$db);
	}
}