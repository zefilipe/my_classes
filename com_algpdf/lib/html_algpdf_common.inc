<?php

/**
 * Html commom methods.
 * 
 * This script, contains a class with common methods to output html.
 * 
 * @author José Filipe Lopes Santos <jfilipe@med.up.pt>
 * @version 1.0 - Date last modified: 31-03-2008
 * @package algpdf
 * @subpackage libraries
 */

// ensure this file is being included by a parent file
defined("_VALID_MOS") or die("Direct Access to this location is not allowed.");

/**
 * Html common methods.
 * 
 * This class, has a common methods to output html.
 */
class html_algpdf_common {
	
	/**
	 * Handler of the database.
	 * @var array
	 */
	var $db = array();
	
	
	/**
	 * Method constructor.
	 *
	 * @param array $db handler of the database
	 * @return html_algpdf_common
	 */
	function html_algpdf_common($db = array()){
		
		$this->db = $db;
	}
	
	
	/**
	 * Creates a patTemplate class instance.
	 *
	 * @return patTemplate
	 */
	function &createTemplate() {
   		global $option, $mosConfig_absolute_path;
    	
        require_once($mosConfig_absolute_path."/includes/patTemplate/patTemplate.php");
 
        $t = &new patTemplate();
        $t->setRoot(dirname(__FILE__)."/../templates");
        return $t;
    }
    
    
    /**
     * Write the yes/no options.
     * 
     * This method, write the yes/no options.
     *
     * @param handler $t handler of the templates
     * @param string $template_name name of the template
     * @param string $value value of the option selected
     */
    function write_yesno_options($t,$template_name,$value){
    	
    	$no = _CMN_NO; // no string
    	$yes = _CMN_YES; // yes string
    	// yes / no array of the options
    	$array_yesno = array("0" => $no, "1" => $yes);
    	$entries = array(); // array with options entries
    	$n = 0; // counter of the entries
    	
    	foreach ($array_yesno as $key=>$text){ 
    		
    		// verify if option is selected
    		$selected = "no";
    		if ($key == $value) $selected = "yes";
    		
    		// append entries to array
    		$entries[$n]["option_selected"] = $selected;
    		$entries[$n]["option_value"] = $key;
    		$entries[$n]["option_text"] = $text;
    		
    		// increment the counter
    		$n++;    		
    	}
    	
    	// write these options
    	$t->addRows($template_name,$entries);
    }
    
    
    /**
     * Write the categories options.
     * 
     * This method, write the categories options.
     *
     * @param handler $t handler of the templates
     * @param string $template_name name of the template
     * @param string $str_values values (ids) of the options selected
     */
    function write_categories_options($t,$template_name,$str_values){
    	
    	$error_msg = "";
    	$entries = array();
    	$n = 0;    	
    	
    	// the $str_values is a strig of the type: value1,value2, ..., valueN (these values are IDs of categories)
    	// and to compare, it will to be explode these values to comparasion effects
    	$values = explode(",",$str_values);
    	
    	// extract data of the categorias
    	$sql = "select c.id as id_cat, c.section as id_section, concat(s.title,' / ',c.title) as name_cat ";
    	$sql .= "from #__categories c, #__sections s ";
    	$sql .= "where c.section=s.id and c.published=1 and s.published=1 ";
    	$sql .= "order by s.title, c.title";
    	    	
    	$this->db->setQuery($sql);
    	$rows = $this->db->loadObjectList();
    	
    	if ($this->db->getErrorNum()){    		
    		$error_msg = $this->db->stderr();
    		
    		// set entries with the error msg
    		$entries[0]["option_selected"] = "yes";
    		$entries[0]["option_value"] = "-1";
    		$entries[0]["option_text"] = $error_msg;
    		
    	} else {
    		
    		foreach ($rows as $index=>$row){ // for each categorie
    			
    			// verify if option is selected
    			$selected = "no";
    			if (in_array($row->id_cat,$values)) $selected = "yes";
    			
    			// append entries to array
    			$entries[$n]["option_selected"] = $selected;
    			$entries[$n]["option_value"] = $row->id_cat;
    			$entries[$n]["option_text"] = $row->name_cat;
    			
    			// increment the counter
    			$n++;    			
    		}
    	}
    	    	
    	// write these options
    	$t->addRows($template_name,$entries);    	
    }
    
    
    /**
     * Write the contents options.
     * 
     * This method, write the contents options, of these categories.
     *
     * @param handler $t handler of the templates
     * @param string $template_name name of the template
     * @param string $value value of the option selected
     * @param string $str_cats string with ids of the categories
     */    
    function write_contents_options($t,$template_name,$value,$str_cats){
    	
    	$error_msg = "";
    	$entries = array();
    	$n = 0;
    	
    	// extract data of the contents
    	$sql = "select id, title from #__content ";
    	$sql .= "where catid in ($str_cats) and state=1 order by title";
    	
    	$this->db->setQuery($sql);
    	$rows = $this->db->loadObjectList();
    	
    	if ($this->db->getErrorNum()){
    		$error_msg = $this->db->stderr();
    		
    		// set entries with the error msg
    		$entries[0]["option_selected"] = "yes";
    		$entries[0]["option_value"] = "-1";
    		$entries[0]["option_text"] = $error_msg;
    		
    	} else {
    		
    		foreach ($rows as $index=>$row){ // for each content
    			
    			// verify if option is selected
    			$selected = "no";
    			if ($value == $row->id) $selected = "yes";
    			
    			// append entries to array
    			$entries[$n]["option_selected"] = $selected;
    			$entries[$n]["option_value"] = $row->id;
    			$entries[$n]["option_text"] = $row->title;
    			
    			// increment the counter
    			$n++;
    		}
    	}
    	
    	// write these options
    	$t->addRows($template_name,$entries);
    }
}

?>