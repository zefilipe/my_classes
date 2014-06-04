<?php

/**
 * Pdf utilities.
 * 
 * This script, contains a class with methods, to create pdf files.
 * 
 * @author José Filipe Lopes Santos <jfilipe@med.up.pt>
 * @version 1.0 - Date last modified: 13-05-2008
 * @package algpdf
 * @subpackage libraries
 */

/**
 * Pdf utilities.
 * 
 * This class, has methods, to create pdf files.
 */
class pdf_utils {
	
	/**
	 * Open file with newsletter in html format, in write mode.
	 * 
	 * This method, open file with newsletter in html format, in write mode.
	 *
	 * @return pointer $fp file pointer of the newsletter
	 */
	function open_news_html(){
		
		// open file with newsletter in html format, in write mode
		$fp = fopen(FILE_NEWS_HTML_PATH,"w") or die("Cant open file <b>".FILE_NEWS_HTML_PATH."</b> to write !");
					
		return $fp;
	}	
	
	
	/**
	 * Append template.
	 * 
	 * This method, append template, to file with newsletter in html format.
	 *
	 * @param pointer $fp file pointer of the newsletter
	 * @param string $template path of the template
	 * @param array[optional] $vars array with the variables
	 */
	function append_template($fp,$template,$vars = array()){
						
		// get the html to this template
		$html_template = file_get_contents($template) or die("Cant open file <b>$template</b> to read !");

		// replace variables to its values, and the template has variables
		if (count($vars) > 0 && eregi("\{[a-z_0-9]+\}",$html_template)){
						
			foreach ($vars as $name_var=>$value_var) // for each variable
				// replace variable to it value
				$html_template = eregi_replace("\{$name_var\}",$value_var,$html_template);
				
		}
				
		// appends the html of this template
		fputs($fp,$html_template);		
	}
	
	
	/**
	 * Append html of the content.
	 * 
	 * This method, append html of the content.
	 *
	 * @param pointer $fp file pointer of the newsletter
	 * @param string $html html of the content
	 */
	function append_content($fp,$html){		
		
		// appends a slash ('/') on begin of the attributes href e src
		// to indicades that begins on the root of the application,
		// except in the cases it contais http:// or www.
		if (eregi("(href|src)(=\")([a-z0-9_-]+)",$html,$regs) && !ereg("(http|www)",$regs[3]))
			$html = eregi_replace("(href|src)(=\")([a-z0-9_-]+)","\\1\\2/\\3",$html);
			
		// appends the html of the content
		fputs($fp,$html);
	}
	
	
	/**
	 * Close file with newsletter in html format.
	 * 
	 * This method, close file with newsletter in html format.
	 *
	 * @param pointer $fp file pointer of the newsletter
	 */
	function close_news_html($fp){
		
		// close file with newsletter in html format
		fclose($fp) or die("Cant close this file !");
	}
	
	
	/**
	 * Generate the pdf.
	 * 
	 * This method, generate the pdf file of their html.
	 *
	 * @param string $html_url url of the html file
	 * @param string $header header of the pdf
	 */
	function generatePDF($html_url,$header){

		// trim all whitespaces and special chars, of the header
		$header = trim($header);
		if (ereg("[^a-z0-9 \.\-]+",$header))
			$header = eregi_replace("[^a-z0-9 \.\-]+","",$header);		
		
		// media settings
		$media = Media::predefined("A4");
		$media->set_landscape(false);
		$media->set_margins(array("left"   => 30,
	        	                  "right"  => 15,
		                          "top"    => 15,
		                          "bottom" => 15));
		$media->set_pixels(1024); 
	
		// global settings
		$GLOBALS["g_config"] = array(
	    	"cssmedia"			=> "screen",
	        "renderimages" 		=> true,
	        "renderforms"  		=> false,
	        "renderlinks"  		=> true,
	        "renderfields"  	=> true,
	        "mode"         		=> "html",
	        "debugbox"     		=> false,
	        "draw_page_border" 	=> false
	    );
	
	    // determine scale
		$g_px_scale = mm2pt($media->width() - $media->margins["left"] - $media->margins["right"]) / $media->pixels;
		$g_pt_scale = $g_px_scale * 1.43; 
	
		$pipeline = new Pipeline;
	    $pipeline->configure($GLOBALS["g_config"]);
		$pipeline->fetchers[] = new FetcherUrl();
		$pipeline->destination = new DestinationFile(FILENAME_PDF); // set a destination pdf file
		$pipeline->data_filters[] = new DataFilterHTML2XHTML;
		$pipeline->pre_tree_filters = array();
		$header_html = $header;
		$footer_html = "";
		$filter = new PreTreeFilterHeaderFooter($header_html, $footer_html); // header and footer
		$pipeline->pre_tree_filters[] = $filter;
		$pipeline->pre_tree_filters[] = new PreTreeFilterHTML2PSFields();
		$pipeline->parser = new ParserXHTML();
		$pipeline->layout_engine = new LayoutEngineDefault;
		$pipeline->output_driver = new OutputDriverFPDF($media);
	
		$pipeline->process($html_url, $media);			
	}
	
	
	/**
	 * Join attachemnents to pdf.
	 * 
	 * This method, join attachements to pdf with newsletter and its news.
	 *
	 * @param string $pdf_complete complete pdf file (newsletter, news and attachs)
	 * @param string $pdf pdf file with newsletter and its news
	 * @param array $attachs array with the attachements
	 */
	function JoinAttachsToPdf($pdf_complete,$pdf,$attachs){
		
		// if pdf is not found then print's a error message and exit
		if (!file_exists($pdf)) die("Pdf <b>$pdf</b> not found !");
		
		// if any attach is not found then print's a error message and exit 
		foreach ($attachs as $attach){			
			if (!file_exists($attach))
				die("Attach <b>$attach</b> not found");				
		}
			
		// join attachs to pdf, using the command gs (ghostscript)
		exec(DEFAULT_GS_PATH."gs -q -sPAPERSIZE=letter -dNOPAUSE -dBATCH -sDEVICE=pdfwrite -sOutputFile=$pdf_complete $pdf ".implode(" ",$attachs));
		
	}
}

?>