<?php

/**
 * Configurações da aplicação.
 * 
 * @author José Filipe Lopes Santos <jfilipe@med.up.pt>
 * @since 23-04-2010
 * @version 2.0 - data da últ. actualização: 10-05-2012
 * @package tts
 * @subpackage configurations
 */


//**************** Paths (caminhos) e Urls *******************

////////// paths e urls principais /////////
/**
 * Url para o diretório principal.
 */
define("ROOT_URL","/my_classes/SendFormattedMail/");
/**
 * caminho para o directório principal.
 */
define("ROOT_PATH",getenv("DOCUMENT_ROOT").ROOT_URL);


//////////// paths para directórios /////////
/**
 * Caminho para o directório das configurações.
 */
define("CONF_PATH",ROOT_PATH."conf/");
/**
 * Caminho para o directório com os includes / libreries.
 */
define("LIB_PATH",ROOT_PATH."lib/");
/**
 * Caminho para o directório das templates.
 */
define("TEMPLATES_PATH",ROOT_PATH."templates/");


////////// URLs para directórios /////////////
/**
 * Url para o directório de estilos.
 */
define("CSS_URL",ROOT_URL."css/");
/**
 * Url para o directório de imagens.
 */
define("IMAGES_URL",ROOT_URL."images/");
/**
 * Url para o directório de scripts em javascript.
 */
define("JS_URL",ROOT_URL."javascript/");


// **************** Includes ************
require_once("DB.php");
require_once("HTML/Template/PHPLIB.php");
require_once 'Mail.php';
require_once 'Mail/mime.php';


// *************** Misc *************
/**
 * Mensagem que é mostrada, quando existem campos obrigatórios por preencher.
 */
define("MSG_EMPTY","Os campos a * não estão preenchidos e, são de preenchimento obrigatório");
/**
 * Símbolo correspondente ao campo preenchido.
 */
define("SYMBOL_NORMAL"," ");
/**
 * Símbolo correspondente ao campo que está por preencher.
 */
define("SYMBOL_EMPTY","*");
/**
 * Mudança de linha nos cabeçalhos do e-mail.
 */
define("CRLF","\r\n");
?>
