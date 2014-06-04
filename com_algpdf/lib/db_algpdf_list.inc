<?php

/**
 * Database listing of the generated pdfs.
 * 
 * This script, contains a class with methods, to manipulate data in daatabase for listing of the generated pdfs.
 * 
 * @author José Filipe Lopes Santos <jfilipe@med.up.pt>
 * @version 1.0 - Date last modified: 27-03-2008
 * @package algpdf
 * @subpackage libraries
 */

// ensure this file is being included by a parent file
defined("_VALID_MOS") or die("Direct Access to this location is not allowed.");


/**
 * Database listing of the generated pdfs.
 *
 * This class, has methods, to manipulate data in database for listing of the generated pdfs.
 */
class db_algpdf_list {

	/**
	 * Handler of the database.
	 * @var array
	 */
	var $db = array();	

	/**
	 * Method constructor.
	 *
	 * @param array $db handler of the database
	 * @return db_algpdf_list
	 */
	function db_algpdf_list($db){

		$this->db = $db;
	}
	
	
	/**
	 * Get data of the generated files.
	 * 
	 * This method, get data of the generated files.
	 *
	 * @param string[optional] $str_ids string with ids of the files
	 * @return array $rows results
	 */
	function getFilesData($str_ids = ""){
		
		$rows = array();
		
		// extract data from generated files
		$sql = "select f.*,c.title as title_content ";
		$sql .= "from #__algpdf_pdfs f, #__content c ";
		$sql .= "where f.id_content = c.id ";
		if ($str_ids != "") $sql .= "and f.id in ($str_ids) ";
		$sql .= "order by ifnull(file_created,now()) desc, file_name asc";
		
		$this->db->setQuery($sql);
		$rows = $this->db->loadObjectList();
		
		if ($this->db->getErrorNum()){
			echo $this->db->stderr();
			return $rows;
		}
		
		return $rows;		
	}
	
	
	/**
	 * Delete files.
	 * 
	 * This method, delete the selected files.
	 *
	 * @param array $ids array with ids
	 * @return false - if error ocurrs; true - otherwise
	 */
	function DeleteFiles($ids){
				
		// delete the data of the files
		$str_ids = implode(",",$ids);
		$delete = "delete from #__algpdf_pdfs where id in ($str_ids)";
		$this->db->setQuery($delete);
		
		if (!$this->db->query()){
			echo "<script> alert('".$this->db->getErrorMsg()."'); window.history.go(-1);</script>n";
			return false;
		}
		
		return true;
	}
}

?>