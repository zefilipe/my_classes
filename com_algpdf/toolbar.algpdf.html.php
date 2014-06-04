<?php

/**
 * Class to Backend Toolbar.
 * 
 * @author José Filipe Lopes Santos <jfilipe@med.up.pt>
 * @version 1.0 - Date last modified: 11-03-2008
 * @package algpdf
 * @subpackage toolbar
 */

/**
 * This classe generates a toolbar.
 */
class toolbar_algpdf {
	
	/**
	 * Creates a toolbar by default.
	 */
	function default_toolbar(){

		mosMenuBar::startTable();
		mosMenuBar::help("algpdf.html",true);
		mosMenuBar::endTable();
	}
	
	/**
	 * Creates a toolbar to configuration tasks.
	 */
	function configuration_toolbar(){
		
		mosMenuBar::startTable();
		mosMenuBar::save("save");
		mosMenuBar::spacer();
		mosMenuBar::cancel("cancel");
		mosMenuBar::spacer();
		mosMenuBar::help("algpdf.html",true);
		mosMenuBar::endTable();
	}	
	
	/**
	 * Creates a toolbar to listing.
	 */
	function listing_toolbar(){
		
		mosMenuBar::startTable();
		mosMenuBar::deleteList("","remove","Remove");
		mosMenuBar::spacer();
		mosMenuBar::help("algpdf.html",true);
		mosMenuBar::endTable();
	}
}

?>