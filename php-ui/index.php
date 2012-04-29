<?php

/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * WebUI for neeko's shiren keygen.
 * Copyright (C) 2010 Patrick Geary
 */


/*
 * We just quickly generate a UI here, and the happy little iframe that
 * the output goes into! yay!  This would probably be perfectly fine
 * without requiring php to interpret it, but whatever.
 */

$header = "
<html>
<title>Revive Keygen for 'Shiren the Wanderer - Mystery Dungeon'</title>
<LINK REL=StyleSheet HREF='stw.css'>
<script type='text/javascript'>
function minium_length_check(myField,minimum) {
	var myForm=document.getElementsByTagName('form'][0];
	if (myNbr.length >= minimum) {
		return;
	} else {
		alert( 'MUST be at least ' + minimum + ' characters in length');
		myField.focus();
		myField.select();
		return;
}
</script>
	  ";

$body = "
<body id='stw_b' style='background: #000000 url(shiren.jpg) no-repeat top center'>

<center>

<div class='comment_d'>
<p style='color:black'>Revive key generator for 
<p style='color:darkred'>Shiren the Wanderer - Mystery Dungeon</p>
<p align='right' style='color:black; font-size:8pt'><br><br>By: nko</p>
</div>


<table id='stw_t'>
  <form name='stw_form' method='post' action='output.php' target='stw_out'>
	<tr id='stw_tr_1'>
		<td id='stw_td1_1'><input name='f1_1' type='text' maxlength='5' size='5' onblur='javascript:minimum_length_check(stw_form.f1_1.value,5)'></td>
		<td id='stw_td1_2'><input name='f1_2' type='text' maxlength='4' size='4' onblur='javascript:minimum_length_check(stw_form.f1_2.value,4)'></td>
		<td id='stw_td1_3'><input name='f1_3' type='text' maxlength='5' size='5' onblur='javascript:minimum_length_check(stw_form.f1_3.value,5)'></td>
	</tr>
		
	<tr id='stw_tr_2'>
		<td id='stw_td2_1'><input name='f2_1' type='text' maxlength='5' size='5' onblur='javascript:minimum_length_check(stw_form.f2_1.value,5)'></td>
		<td id='stw_td2_2'><input name='f2_2' type='text' maxlength='4' size='4' onblur='javascript:minimum_length_check(stw_form.f2_2.value,4)'></td>
		<td id='stw_td2_3'><input name='f2_3' type='text' maxlength='5' size='5' onblur='javascript:minimum_length_check(stw_form.f2_3.value,5)'></td>
	</tr>
	
	<tr id='stw_tr_3'>
		<td id='stw_td3_1'><input name='f3_1' type='text' maxlength='5' size='5' onblur='javascript:minimum_length_check(stw_form.f3_1.value,5)'></td>
		<td id='stw_td3_2'><input name='f3_2' type='text' maxlength='4' size='4' onblur='javascript:minimum_length_check(stw_form.f3_2.value,4)'></td>
		<td id='stw_td3_3'><input name='f3_3' type='text' maxlength='5' size='5' onblur='javascript:minimum_length_check(stw_form.f3_3.value,5)'></td>
	</tr>

	<tr id='stw_tr_4'>
		<td id='stw_td4_1'><input name='f4_1' type='text' maxlength='5' size='5' onblur='javascript:minimum_length_check(stw_form.f4_1.value,5)'></td>
		<td id='stw_td4_2'><input name='f4_2' type='text' maxlength='4' size='4' onblur='javascript:minimum_length_check(stw_form.f4_2.value,4)'></td>
		<td id='stw_td4_3'><input name='f4_3' type='text' maxlength='5' size='5' onblur='javascript:minimum_length_check(stw_form.f4_3.value,5)'></td>
	</tr>
	
    <tr id='stw_tr_5'>
        <td id='stw_td5_1'></td>
        <td id='stw_td5_2'><input type='submit' name='submit' value='submit'></td>
        <td id='stw_td5_3'></td>
    </tr>
	
  </form>
</table>

</body>

<iframe name='stw_out' id='frame' src='output.php' width='15%' height='20%' frameborder='0'></iframe>
		";

$footer = "</html>";

$html = $header . $body . $footer;

echo $html;
?>
