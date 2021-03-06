<?php

/**
 * Configura��es da aplica��o.
 * 
 * @author Jos� Filipe Lopes Santos <jfilipe@med.up.pt>
 * @since 23-04-2010
 * @version 2.0 - data da �lt. actualiza��o: 10-05-2012
 * @package tts
 * @subpackage configurations
 */


//**************** Paths (caminhos) e Urls *******************

////////// paths e urls principais /////////
/**
 * Url para o diret�rio principal.
 */
define("ROOT_URL","/my_classes/SendFormattedMail/");
/**
 * caminho para o direct�rio principal.
 */
define("ROOT_PATH",getenv("DOCUMENT_ROOT").ROOT_URL);


//////////// paths para direct�rios /////////
/**
 * Caminho para o direct�rio das configura��es.
 */
define("CONF_PATH",ROOT_PATH."conf/");
/**
 * Caminho para o direct�rio com os includes / libreries.
 */
define("LIB_PATH",ROOT_PATH."lib/");
/**
 * Caminho para o direct�rio das templates.
 */
define("TEMPLATES_PATH",ROOT_PATH."templates/");


////////// URLs para direct�rios /////////////
/**
 * Url para o direct�rio de estilos.
 */
define("CSS_URL",ROOT_URL."css/");
/**
 * Url para o direct�rio de imagens.
 */
define("IMAGES_URL",ROOT_URL."images/");
/**
 * Url para o direct�rio de scripts em javascript.
 */
define("JS_URL",ROOT_URL."javascript/");


// **************** Includes ************
require_once("DB.php");
require_once("HTML/Template/PHPLIB.php");
require_once 'Mail.php';
require_once 'Mail/mime.php';


// *************** Misc *************
/**
 * Mensagem que � mostrada, quando existem campos obrigat�rios por preencher.
 */
define("MSG_EMPTY","Os campos a * n�o est�o preenchidos e, s�o de preenchimento obrigat�rio");
/**
 * S�mbolo correspondente ao campo preenchido.
 */
define("SYMBOL_NORMAL"," ");
/**
 * S�mbolo correspondente ao campo que est� por preencher.
 */
define("SYMBOL_EMPTY","*");
/**
 * Mudan�a de linha nos cabe�alhos do e-mail.
 */
define("CRLF","\r\n");
?>
