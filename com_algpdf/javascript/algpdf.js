/********************************************
* ALGPDF - Analyze Links and Generate PDF   *
* 	    Algpdf common functions			    *
*											*
* Author: José Filipe Lopes Santos			*
* Date last modified: 19-05-2008			*
*											*
*********************************************/

/**
Open a popup / window.

Parameters:
	file			- 	file
	window_width 	- 	width of the window
	window_height	-	height of the window
	window_name		-	name of the window
*/
function open_window(file,window_width,window_height,window_name){
	var win = window.open(file,window_name,"toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=yes,width="+window_width+",height="+window_height);
}


/**
Change the string, based in options selected.

Parameters:
	f			-	form
	listbox		-	listbox / select multiple
	fiel_str	-	field with the string of values
*/
function change_str(f,listbox,field_str){
	
	var str = "";
	
	for (var i=0;i<listbox.length;i++){
		if (listbox.options[i].selected) // if option selected
			// append this value to string
			str = str + "," + listbox.options[i].value;
	} 	
	
	// cut the first ';'
	if (str != "")
		str = str.substring(1,str.length)
		
	// save string to field
	eval("document."+f.name+"."+field_str+".value = '"+str+"'"); 
}


/**
Overwrite the submitbutton function, to validate fields of the configuration.

Parameters:
	pressbutton	-	value of the pressed button
*/
function submitbutton(pressbutton) {

	// get a variable with the form
	var form = document.adminForm;
	
	// if button pressed is not 'save', submit the form data, without restrictions
	if (pressbutton != "save"){
		submitform(pressbutton);
		return;
	}

	// do field validation
	try {
		form.onsubmit();
	} 
	catch(e) {}

	// check if has empty fields, when that is obligatory
	if (isEmpty(form,"path_storefiles"))
		alert("Must have the Path for storing files");
	else if (isEmpty(form,"analyze_cats"))
		alert("Must have the Categories to analyze");
	else
		submitform(pressbutton);
}


/**
Verify if the field is empty.

Parameters:
	form		-	form in use
	fieldname	-	name of the field
*/
function isEmpty(form,fieldname){
	
	// gets the value of these field
	var value = eval("document."+form.name+"."+fieldname+".value");

	if (value == "") return true;
	else return false;
}


/**
* Submit data to analyze links.
*
* @param handler f form in use
* @param string listbox_content listbox to select content
* @param string descr description of the listbox to select content
* @param string field_name_pdf field with the name of the pdf
* @param string descr_name_pdf description of the field with the name of the pdf
* @param string listbox_overwrite listbox to select overwrite pdf options
* @param string field_step name of the hidden with the step
* @param string field_pdf_ok name of the hidden with a flag that identifies if the name of the pdf is ok
* @param string field_submited name of the hidden with a flag that identifies if the data is submited
*/
function AnalyzeLinks(f,listbox_content,descr,field_name_pdf,descr_name_pdf,listbox_overwrite,field_step,field_pdf_ok,field_submited){

	// set variable with the listbox
	var listbox = eval("document."+f.name+"."+listbox_content);
	
	// number of the selected items
	var n_selected = 0; 
	
	// count the number of the options selected
	for (var i = 0; i < listbox.length; i++){
		if (listbox.options[i].selected)
			n_selected++;
	}
	
	if (n_selected == 0) // no options selected
		alert("Must have the "+descr);
	else if (isEmpty(f,field_name_pdf))
		alert("Must have the "+descr_name_pdf);	
	else {
	
		// get values
		var step = eval("document."+f.name+"."+field_step+".value");
		var ok = eval("document."+f.name+"."+field_pdf_ok+".value");
		
		// get listbox object, of the name of the pdf
		var listbox2 = eval("document."+f.name+"."+listbox_overwrite);
		
		// get value of the overwrite flag
		var overwrite = 0;
		for (var i=0; i<listbox2.length; i++){
			if (listbox2.options[i].selected){
				overwrite = listbox2.options[i].value;
				break;
			}
		} 
		
		// if name of pdf is not empty and not overwrite and is not ok => then back 1 step to modify name of the pdf
		if (overwrite == 0 && ok == "n"){
			step = step -1;
			eval("document."+f.name+"."+field_step+".value = '"+step+"'");		
		}
		
		// modify submited flag
		eval("document."+f.name+"."+field_submited+".value = 'y'");
				
		f.submit(); // submit data
	}
}


/**
Stop loading.

This function stop loading, when a page is loaded, modifying text and img that indicates this is loading, 
and enable button to continue. 
*/
function stopLoading(){

	// settings
	var layer_text = "loading_label";
	var layer_img = "loading_img";
	var formname = "analyzing";
	var buttonname = "continue_bt";

	// changes the text and the image in these layers
	if (document.all){ // IE
	
		document.all(layer_text).innerHTML = "<h1 align='center'>Page loaded</h1>";
		document.all(layer_img).innerHTML = "";
		
	} else if (document.getElementById) { // Firefox and others
	
		document.getElementById(layer_text).innerHTML = "<h1 align='center'>Page loaded</h1>";
		document.getElementById(layer_img).innerHTML = "";
	}
	
	// enable the button and modify foregroud color
	eval("document."+formname+"."+buttonname+".disabled = false");
	eval("document."+formname+"."+buttonname+".style.color = '#000000'");
}


/**
Stop loading 2.

This function stop loading, when a page is loaded, modifying text and img that indicates this is loading, 
and enable button to finish. 
*/
function stopLoading2(){

	// settings
	var layer_text = "loading_label";
	var layer_img = "loading_img";
	var formname = "gen";
	var buttonname = "finish";

	// changes the text and the image in these layers
	if (document.all){ // IE
	
		document.all(layer_text).innerHTML = "<h1 align='center'>Page loaded</h1>";
		document.all(layer_img).innerHTML = "";
		
	} else if (document.getElementById) { // Firefox and others
	
		document.getElementById(layer_text).innerHTML = "<h1 align='center'>Page loaded</h1>";
		document.getElementById(layer_img).innerHTML = "";
	}
	
	// enable the button and modify foregroud color
	eval("document."+formname+"."+buttonname+".disabled = false");
	eval("document."+formname+"."+buttonname+".style.color = '#000000'");
}


/**
* Toggles the check state of a group of checkboxes, in multiple listings.
*
* @param object f form in use
* @param string prefix_checks prefix used in these checkboxes
* @param string field_checkall field with the checkbox to check/uncheck all
* @param string field_boxchecked field hidden to store number of checkboxes checked
*/
function checkAllMultiple(f,n,prefix_checks,field_checkall,field_boxchecked) {

	// variable with property check of the checkbox to check all
	var checked = eval("document."+f.name+"."+field_checkall+".checked");
	
	var n2 = 0;
	for (i=0; i < n; i++) {
		cb = eval("document."+f.name+"."+prefix_checks+""+i);
		if (cb) {
			cb.checked = checked;
			n2++;
		}
	}
	
	if (checked) 
		eval("document."+f.name+"."+field_boxchecked+".value = "+n2);		
	else 
		eval("document."+f.name+"."+field_boxchecked+".value = 0");	
}


/**
* Verify if this checkbox is checked, in multiple listings.
*
* This function, verify if this checkbox is checked, 
* and update the number of checkboxes checked of this group,
* used in multiple listings.
*
* @param object f form in use
* @param bool is_checked identifies if this checkbox is checked
* @param string field_boxchecked field hidden to store number of checkboxes checked
*/
function isCheckedMultiple(f,is_checked,field_boxchecked){

	// get boxchecked object
	var boxchecked = eval("document."+f.name+"."+field_boxchecked);

	if (is_checked == true) // is checked
	
		// increase number of checkboxes checked, of this group
		boxchecked.value++;
		
	else // is unchecked
	
		// decrease number of checkboxes checked, of this group
		boxchecked.value--;
}


/**
* Submit data from the list links page.
*
* @param object f form in use
* @param string pref_attachs prefix used in checkboxes to attachements
* @param string pref_contents prefix used in checkboxes to contents
* @param integer n_attachs number of the attachements
* @paran integer n_contents number of the contents
*/
function SubmitListLinks(f,pref_attachs,pref_contents,n_attachs,n_contents){

	var n_checks = 0; // number of checkboxes checked
	
	// detect number of checked attachements
	for (var i=0;i < n_attachs; i++){
		if (eval("document."+f.name+"."+pref_attachs+""+i+".checked"))
			n_checks++;
	}
	
	// detect number of checked contents
	for (var i=0;i < n_contents; i++){
		if (eval("document."+f.name+"."+pref_contents+""+i+".checked"))
			n_checks++;
	}
	
	// check if has empty fields, when that is obligatory
	if (n_checks == 0)
		alert("Must have any checkboxes checked");
	else
	
		// submit data
		f.submit();
}


/**
* Set the name of the pdf.
*
* @param handler f formulário em causa
* @param string field_listbox listbox with the selected content
* @param string field_name_pdf textbox with the name of the pdf
* @param string field_submited name of the hidden with a flag that indifies if the data is submited
*/
function setNamePdf(f,field_listbox,field_name_pdf,field_submited){

	// get the values for name of the pdf and submited flag 
	var name_pdf = eval("document."+f.name+"."+field_name_pdf+".value");
	var submited = eval("document."+f.name+"."+field_submited+".value");
	
	// only modifies name of the pdf, if it is not submited any time
	if (submited == "n"){
	
		// get listbox object
		var listbox = eval("document."+f.name+"."+field_listbox);
		
		// get the title of the selected item
		var title = "";
		for (var i=0;i<listbox.length;i++){
			if (listbox.options[i].selected){
				title = listbox.options[i].text;
				break;
			}
		}
		
		// convert string to lower case
		title = title.toLowerCase();	
		
		// translate the title
		title = TranslateString(title);	
		
		// replace all non alphanumeric character or underscore, to underscore ('_')
		var not_word = /\W/g;
		if (not_word.test(title))
			title = title.replace(not_word,"_");
		
		// replace multiple underscores consecutives to a single underscore
		var mult_underscore = /[_]{2,}/g;
		if (mult_underscore.test(title))
			title = title.replace(mult_underscore,"_");
	
		// trims underscores in the begin and in the end of the string
		var not_trimed = /^([_]*)([a-zA-Z0-9]+)(\w*)([a-zA-Z0-9]+)([_]*)$/;
		if (not_trimed.test(title))
			title = title.replace(not_trimed,"$2$3$4");
		
		// store the title normalized in the field with name of the pdf
		eval("document."+f.name+"."+field_name_pdf+".value = '"+title+"'");
	}	
} 


/**
* Translate string.
*
* @param string str initial string
* @return string str string translated
*/
function TranslateString(str){

	// array with the characters replaces
	var chars_replaces = {'á' : 'a', 'é' : 'e', 'í' : 'i', 'õ' : 'o', 'ù' : 'u', 'à' : 'a', 'â' : 'a', 'ê' : 'e', 
						  'î' : 'i', 'ò' : 'o', 'ú' : 'u', 'ã' : 'a', 'ç' : 'c', 'ó' : 'o', 'û' : 'u', 'À' : 'A',
						  'È' : 'E', 'Ì' : 'I', 'Á' : 'A', 'É' : 'E', 'Õ' : 'O', 'Ù' : 'U', 'Â' : 'A', 'Ê' : 'E',
						  'Î' : 'I', 'Ò' : 'O', 'Ú' : 'U', 'Ã' : 'A', 'Ç' : 'C', 'Ó' : 'O', 'Û' : 'U', 'ñ' : 'n'};
	
	for (var c in chars_replaces){ // for each pair (accentued character, substituition)
	
		// replace accentued characters to its substituitions
		str = str.replace(new RegExp(c,"g"),chars_replaces[c]);
	}
	
	return str;
}


/**
* Verify if the field is word.
*
* @param object field field to verify
* @param string descr description of the field 
*/
function verify_word(field,descr){

	// pattern to match not words 
	var not_word = /\W/;

	// verify if the field has not word characters
	if (not_word.test(field.value)){
		alert(descr+" must be only have letters ([a-z]), numbers ([0-9]), and underscores ([_])");
		field.value = "";
	}
}


/**
* set ok.
*
* @param string formname name of the form
* @param string field_ok name of the field with the flag ok
*/
function setOk(formname,field_ok){

	// set flag ok = y and step = step + 1
	eval("document."+formname+"."+field_ok+".value = 'y'");
	eval("document."+formname+".submit()");
}


/**
* Change the location of this window or frame and make reload.
*
* @param object windowname name of the window or frame
* @param string location location to redirect
*/
function ChangeLocation(windowname,location){
	windowname.location = location;
}
