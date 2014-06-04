<?php

/**
 * Configurations.
 * 
 * This script, contains configurations of the component algpdf.
 * 
 * @author José Filipe Lopes Santos <jfilipe@med.up.pt>
 * @version 1.0 - Date last modified: 13-05-2008
 * @package algpdf
 * @subpackage config
 */

//***************** Paths & Urls *************

/////////// Urls ///////////

/**
 * Root url of the component algpdf.
 */
define("ALGPDF_ROOT_URL","/administrator/components/com_algpdf");

/**
 * Javascript url.
 */
define("JAVASCRIPT_URL",ALGPDF_ROOT_URL."/javascript");

/**
 * Images url.
 */
define("IMAGES_URL",ALGPDF_ROOT_URL."/images");

/**
 * Temporary directory url.
 */
define("TMP_URL",ALGPDF_ROOT_URL."/tmp");


///////////// Paths ///////////

/**
 * Root path of the application.
 */
define("APP_ROOT_PATH",getenv("DOCUMENT_ROOT"));

/**
 * Root path of the component algpdf.
 */
define("ALGPDF_ROOT_PATH",APP_ROOT_PATH.ALGPDF_ROOT_URL);

/**
 * Temporary directory path.
 */
define("TMP_PATH",ALGPDF_ROOT_PATH."/tmp");

/**
 * Templates path.
 */
define("TEMPLATES_PATH",ALGPDF_ROOT_PATH."/templates");

/**
 * Html2ps_pdf path.
 */
define("HTML2PS_PDF_PATH",APP_ROOT_PATH."/includes/html2ps_pdf");

/**
 * Output path with the generated pdfs.
 */
define("OUT_PATH",HTML2PS_PDF_PATH."/out");

/**
 * Default ghostscript path.
 * 
 * If gs has multiple installations, set the default path,
 * Otherwise, it may be empty.
 */
define("DEFAULT_GS_PATH","/usr/local/bin/");


//************** Temporary files ***********

/**
 * File with newsletter and its news in html format path.
 */
define("FILE_NEWS_HTML_PATH",TMP_PATH."/newsletter.html");

/**
 * File with newsletter and its news in html format url.
 */
define("FILE_NEWS_HTML_URL",TMP_URL."/newsletter.html");

/**
 * Filename of the pdf.
 */
define("FILENAME_PDF","newsletter");

/**
 * Temporary pdf file with the newsletter and its news.
 */
define("FILE_PDF_TMP",TMP_PATH."/".FILENAME_PDF.".pdf");

/**
 * Pdf file generated with the newsletter and its news.
 */
define("FILE_PDF_OUT",OUT_PATH."/".FILENAME_PDF.".pdf");

/**
 * Temporary complete pdf file (newsletter, news and attachs).
 */
define("FILE_PDF_COMPLETE_TMP",TMP_PATH."/".FILENAME_PDF."_complete.pdf");


//******************** Misc ******************

/**
 * Name of the application.
 */
define("APPLICATION_NAME","boletim");

?>