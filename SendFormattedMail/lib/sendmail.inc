<?php

/**
 * Enviar mails.
 * 
 * Este include, possui uma classe, para enviar mails.
 *
 * @author Jos� Filipe Lopes Santos <jfilipe@med.up.pt>
 * @since 27-10-2010
 * @version 2.0 - data da �lt. actualiza��o: 17-02-2012
 * @package tts
 * @subpackage libraries
 */

/**
 * Enviar mails.
 * 
 * Esta classe, possui diversos m�todos, para enviar mails.
 */
class sendmail {

	/**
	 * Mensagem de erro.
	 * @var string
	 */
	var $error_msg = "";
	
	/**
	 * Booleano que indica se j� foi enviado.
	 * @var boolean
	 */
	var $sended = false;

	/**
	 * Mudan�a de linha nos headers (carriage return line feed).
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
	 * M�todo constructor.
	 *
	 * @param string[optional] $crlf mudan�a de linha nos headers
	 * @return sendmail
	 */	
	function sendmail($crlf = ""){
		
		$this->crlf = $crlf;
	}
	
	
	/**
	 * Adiciona uma template.
	 * 
	 * Este m�todo, adiciona uma template ao array de templates.
	 *
	 * @param string $template_name nome da template
	 * @param string $template_file ficheiro com a template
	 */
	function addTemplate($template_name,$template_file){

		if (!empty($this->templates[$template_name])){
			$this->error_msg = "A template <b>$template_name</b> j� existe !";
			return ;
		}
		
		$this->templates[$template_name] = $template_file;
	}
	
	
	/**
	 * Obtem os dados das templates.
	 * 
	 * Este m�todo, obtem os dados das templates.
	 *
	 * @return array array com os dados das templates
	 */
	function getTemplates(){
		return $this->templates;
	}
	
	
	/**
	 * Adiciona uma imagem.
	 * 
	 * Este m�todo, adiciona uma imagem ao array de imagens.
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
	 * Este m�todo, obtem os dados das imagens.
	 *
	 * @return array array com os dados das imagens
	 */
	function getImages(){		
		return $this->images;
	}
	
	
	/**
	 * Gera a mensagem formatada para enviar por e-mail.
	 * 
	 * Este m�todo, usando templates, gera a mensagem formatada para enviar por e-mail.
	 *
	 * @param string $type tipo de informa��o
	 * @param array $info array com as informa��es
	 * @param string $templates_path caminho para o direct�rio de templates
	 * @param string $title t�tulo
	 * @return string $message mensagem formatada para enviar por e-mail
	 */
	function genFormattedMailMessage($type,$info,$templates_path,$title){
		global $labels_problemas;
		
		$message = "";
		
		if (count($this->templates) == 0){
			$this->error_msg = "Templates n�o especificados";
			return $message;
		}
		
		$t = new Template_PHPLIB($templates_path);					  
		$t->setFile($this->templates);

		if (strcmp("ticket",$type) == 0){ // tipo tt (trouble ticket) completo		
		
			//*********** P�gina (in�cio) *************
			$t->setVar("titulo",$title);

			//********** Info (in�cio) ********
			
			/// setar vari�veis ///
			$t->setVar(array("name_value" => $info["name"],
					 "email_value" => $info["email"],
                                         "subject_value" => $info["subject"],
					 "message_value" => $info["message"]));
			
			//********** info (fim) **********
			$t->parse("block_body","info",true);					
		
			//*********** P�gina (fim) *************
			$t->parse("output","page");
		}
		
		// obter a mensagem em html
		$message = $t->finish($t->get("output"));
		
		return $message;
	}

	
	/**
	 * Envia a mensagem formatada por e-mail.
	 * 
	 * Este m�todo, envia a mensagem formatada por e-mail.
	 *
	 * @param string $message mensagem formatada
	 * @param string $to destinat�rios
	 * @param array $headers cabe�alhos da mensagem
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