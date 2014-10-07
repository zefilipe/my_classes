<?php

/**
 * Enviar mails.
 * 
 * Este include, possui uma classe, para enviar mails.
 *
 * @author José Filipe Lopes Santos <jfilipe@med.up.pt>
 * @since 27-10-2010
 * @version 2.0 - data da últ. actualização: 17-02-2012
 * @package tts
 * @subpackage libraries
 */

/**
 * Enviar mails.
 * 
 * Esta classe, possui diversos métodos, para enviar mails.
 */
class sendmail {

	/**
	 * Mensagem de erro.
	 * @var string
	 */
	var $error_msg = "";
	
	/**
	 * Booleano que indica se já foi enviado.
	 * @var boolean
	 */
	var $sended = false;

	/**
	 * Mudança de linha nos headers (carriage return line feed).
	 * @var string
	 */
	var $crlf = "";

	/**
	 * Array com as imagens.
	 * @var array
	 */
	var $images = array();
	
	/**
	 * Array com as templates.
	 * @var array
	 */
	var $templates = array();
	
	/**
	 * Método constructor.
	 *
	 * @param string[optional] $crlf mudança de linha nos headers
	 * @return sendmail
	 */	
	function sendmail($crlf = ""){
		
		$this->crlf = $crlf;
	}
	
	
	/**
	 * Adiciona uma template.
	 * 
	 * Este método, adiciona uma template ao array de templates.
	 *
	 * @param string $template_name nome da template
	 * @param string $template_file ficheiro com a template
	 */
	function addTemplate($template_name,$template_file){

		if (!empty($this->templates[$template_name])){
			$this->error_msg = "A template <b>$template_name</b> já existe !";
			return ;
		}
		
		$this->templates[$template_name] = $template_file;
	}
	
	
	/**
	 * Obtem os dados das templates.
	 * 
	 * Este método, obtem os dados das templates.
	 *
	 * @return array array com os dados das templates
	 */
	function getTemplates(){
		return $this->templates;
	}
	
	
	/**
	 * Adiciona uma imagem.
	 * 
	 * Este método, adiciona uma imagem ao array de imagens.
	 *
	 * @param string $img_src source da imagem
	 * @param string $img_type tipo da imagem
	 * @param string $img_name nome da imagem
	 */
	function addImage($img_src,$img_type,$img_name){
		
		$c = count($this->images);
		$this->images[$c]["src"] = $img_src;
		$this->images[$c]["type"] = $img_type;
		$this->images[$c]["name"] = $img_name;
	}
	
	
	/**
	 * Obtem os dados das imagens.
	 * 
	 * Este método, obtem os dados das imagens.
	 *
	 * @return array array com os dados das imagens
	 */
	function getImages(){		
		return $this->images;
	}
	
	
	/**
	 * Gera a mensagem formatada para enviar por e-mail.
	 * 
	 * Este método, usando templates, gera a mensagem formatada para enviar por e-mail.
	 *
	 * @param string $type tipo de informação
	 * @param array $info array com as informações
	 * @param string $templates_path caminho para o directório de templates
	 * @param string $title título
	 * @return string $message mensagem formatada para enviar por e-mail
	 */
	function genFormattedMailMessage($type,$info,$templates_path,$title){
		global $labels_problemas;
		
		$message = "";
		
		if (count($this->templates) == 0){
			$this->error_msg = "Templates não especificados";
			return $message;
		}
		
		$t = new Template_PHPLIB($templates_path);					  
		$t->setFile($this->templates);

		if (strcmp("ticket",$type) == 0){ // tipo tt (trouble ticket) completo		
		
			//*********** Página (início) *************
			$t->setVar("titulo",$title);

			//********** Info (início) ********
			
			/// setar variáveis ///
			$t->setVar(array("name_value" => $info["name"],
					 "email_value" => $info["email"],
                                         "subject_value" => $info["subject"],
					 "message_value" => $info["message"]));
			
			//********** info (fim) **********
			$t->parse("block_body","info",true);					
		
			//*********** Página (fim) *************
			$t->parse("output","page");
		}
		
		// obter a mensagem em html
		$message = $t->finish($t->get("output"));
		
		return $message;
	}

	
	/**
	 * Envia a mensagem formatada por e-mail.
	 * 
	 * Este método, envia a mensagem formatada por e-mail.
	 *
	 * @param string $message mensagem formatada
	 * @param string $to destinatários
	 * @param array $headers cabeçalhos da mensagem
	 * @return boolean $sended booleano que indica se a mensagem foi enviada com sucesso
	 */
	function Send($message,$to,$headers){

		$sended = false;
		
		$mime = new Mail_mime($this->crlf);
		
		// adicionar as imagens
		foreach ($this->images as $img)
			$mime->addHTMLImage($img["src"],$img["type"],$img["name"]);

		// enviar a mensagem
		$mime->setHTMLBody($message);
		$body = $mime->get();
		$headers = $mime->headers($headers);
		$mail =& Mail::Factory("mail");
		$res = $mail->send($to,$headers,$body);

		if (PEAR::isError($res)){
			$this->error_msg = $res->getMessage();
			return $sended;
		}
		
		// mensagem enviada com sucesso
		$sended = true;
		return $sended;
	}
}

?>