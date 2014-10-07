<?php

/* form with
 * name
 * email
 * subject
 * message
 * 
 * send to mail ....
 */


/**
 * Send ticket by email.
 * 
 * @author José Filipe Lopes Santos <jfilipe@med.up.pt>
 * @since 09-05-2012
 * @version 1.0 - data da últ. actualização: 10-05-2012
 */

require_once 'conf/core.inc';
require_once 'lib/sendmail.inc';

/* ---------------------------------- oOo --------------------------------
                             Inicialização de variáveis
   ---------------------------------- oOo -------------------------------- */

if (empty($name)) $name = ""; 
if (empty($email)) $email = ""; 
if (empty($subject)) $subject = "";
if (empty($message)) $message = "";
if (empty($note_empty)) $note_empty = false; // booleano que indica se existem campos obrigatórios por preencher
if (empty($error_msg)) $error_msg = ""; // mensagem de erro
if (empty($sended)) $sended = "no"; // indica se já foi enviado ou não o ticket por email
if (empty($title)) $title = "Send a ticket";


/* ---------------------------------- oOo --------------------------------
             Verificar se existem campos obrigatórios por preencher
   ---------------------------------- oOo -------------------------------- */

if (!empty($submit) && ($name == "" || $email == ""))
    $note_empty = true;
else 
    $note_empty = false;


/* ---------------------------------- oOo --------------------------------
                                  Enviar mails
   ---------------------------------- oOo -------------------------------- */

if (!empty($submit) && !$note_empty && $error_msg == ""){

	// remetente / de e destinatário / para
	$from = "Trouble Tickets (tts) <tts@med.up.pt>";
        $to = "jfilipe@med.up.pt";

	// criar uma nova instânca da classe para enviar mails
	$sm = new sendmail(CRLF);

	// adicionar template
	$sm->addTemplate("page","mail_page.html");
        $sm->addTemplate("info","info_ticket.html");
	
	// mensagem de erro
	if ($sm->error_msg != "") die("erro: ".$sm->error_msg."\n");

        // adicionar imagem
        $sm->addImage("images/cabecalho.gif","image/gif","cabecalho.gif");
                
	// determinar os cabeçalhos
	$subject_mail = "Ticket: ".$subject;        
	$headers = array("From" => $from, "Subject" => $subject_mail);

        // Information to formatted message
        // can be get from database record
        $info = array("name" => $name, "email" => $email, "subject" => $subject, "message" => $message);
                
	// obter a mensagem formatada                
        $message = $sm->genFormattedMailMessage("ticket",$info,TEMPLATES_PATH,$subject_mail);

	// mensagem de erro
	if ($sm->error_msg != "") die("erro: ".$sm->error_msg."\n");

	// enviar a mensagem
	$sm->Send($message,$to,$headers);
	
	// mensagem de erro
	if ($sm->error_msg != "") die("erro: ".$sm->error_msg."\n");		
        
        $sended = "yes";
}

/* ---------------------------------- oOo --------------------------------
				     Output
   ---------------------------------- oOo -------------------------------- */


$t = new Template_PHPLIB(TEMPLATES_PATH);			  
			  
$t->setFile(array("page" => "page.html",
                  "form" => "form_ticket.html",
                  "note" => "note_without_table.html",
		  "note2" => "note_without_table.html"));				  				  


//*********** Página (início) *************
$t->setVar(array("rooturl" => ROOT_URL, "titulo" => $title));


if (ereg("yes",$sended)){ // enviado

    //*********** mensagem de sucesso *******
    $t->setVar("msg","Ticket sended with sucess");
    $t->parse("block_body","note",true);    
    
} else {

    //////// destacar os campos por preencher ///////////
    if (!empty($submit) && $name == "") $t->setVar("symbol_name",SYMBOL_EMPTY);
    else $t->setVar("symbol_name",SYMBOL_NORMAL);
    if (!empty($submit) && $email == "") $t->setVar("symbol_email",SYMBOL_EMPTY);
    else $t->setVar("symbol_email",SYMBOL_NORMAL);

    /////// setar variáveis //////
    $t->setVar(array("name_value" => $name,
                     "email_value" => $email,
                     "subject_value" => $subject,
                     "message_value" => $message));


    //************* mensagem de erro ***********
    if ($error_msg == "" && $note_empty) $error_msg = MSG_EMPTY;
    if ($error_msg != ""){
            $t->setVar(array("colspan_note" => 2, "msg" => $error_msg));
            $t->parse("block_note","note2",true);
    }

    //********** formulário (fim) **********
    $t->parse("block_body","form",true);

}

//*********** Página (fim) *************
$t->pparse("output","page");

?>
