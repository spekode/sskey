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
 * This file does the actual call to the generator, then prints the output.
 */

/* First, let's strip apart that nasty post data to make sure that they're not
 * using any special chars. (in order)
 * - non-'word' chars
 * - whitespaces
 * - extended unicode
 * - unicode
 * - no value whatsoever
 */

//Test it! if it fails return an error message.

if(!sizeof($_POST)) {
	die(' ');
}

foreach ($_POST as $input) { 
	if (preg_match('/[\#\&\;\`\|\*\?\~\<\>\^\(\)\\\[\]\{\}\$\,\"\ ]/', $input)) {
		die("<html><body><div align='center' style='background-color: #000000; border: 1px solid white; opacity:0.6'><p style='font-weight:bold; color:red'>Invalid Input</p></div></body></html>");
	}
}


//Extract only the fields we want.

$wanted = '/^f\d_\d$/';
foreach ($_POST as $key => $input) {
	if (preg_match($wanted, $key)) {
		$data[$key] = strtoupper($input);
		$data[$key] = escapeshellcmd($data[$key]);
	}
}


/*
 * Format the input to something that the system call will play with. 
 */
ksort($data);
$input_data = implode("", $data);

/* 
 * Let's make that system call
 */

$rawout = shell_exec('echo ' . $input_data . ' | /var/www/sskey/sskey -r');

//Die if it fails to retrieve anything
if(!isset($rawout)) {
	die("<html><body><div align='center' style='background-color: #000000; border: 1px solid white; opacity:0.6'><p style='font-weight:bold; color:red'>Invalid Input</p></div></body></html>");
}

/*
 * Time to build the output arrays
 */

//Strip slashes
$output = trim($rawout);

//Replace all the newlines with spaces
$output = preg_replace('/\n/', ' ', $output);

//Remove occurances of multiple whitespaces
$output = preg_replace('/\ /', ' ', $output);

//Turn the string into an array.
$output = explode(" ", $output);

/*
 * Create the body table in html, this should be cleaner, this was quicker.
 */
foreach ($output as $key => $data) {
	if ($key == 0) {
		$body[] = "<tr id='stw_o_tr_1'><td id='stw_o_td1_1'>" . $data . "</td>";
	} elseif ($key == 1) {
		$body[] = "<td id='stw_o_td1_2'>" . $data . "</td>";	
	} elseif ($key == 2) {
		$body[] = "<td id='stw_o_td1_3'>" . $data . "</td></tr>";
	} elseif ($key == 3) {
		$body[] = "<tr id='stw_o_tr_2'><td id='stw_o_td2_1'>" . $data . "</td>";
	} elseif ($key == 4) {
		$body[] = "<td id='stw_o_td2_2'>" . $data . "</td>";
	} elseif ($key == 5) {
		$body[] = "<td id='stw_o_td2_3'>" . $data . "</td></tr>";
	} elseif ($key == 6) {
		$body[] = "<tr id='stw_o_tr_3'><td id='stw_o_td3_1'>" . $data . "</td>";
	} elseif ($key == 7) {
		$body[] = "<td id='stw_o_td3_2'>" . $data . "</td>";
	} elseif ($key == 8) {
		$body[] = "<td id='stw_o_td3_3'>" . $data . "</td></tr>";
	} elseif ($key == 9) {
		$body[] = "<tr id='stw_o_tr_4'><td id='stw_o_td4_1'>" . $data . "</td>";
	} elseif ($key == 10) {
		$body[] = "<td id='stw_o_td4_2'>" . $data . "</td>";
	} elseif ($key == 11) {
		$body[] = "<td id='stw_o_td4_3'>" . $data . "</td></tr>";
	}
}

//turn the body array into a string.
$tables = implode("", $body);

/*
 * Normal html time.
 */

$header = "<html><LINK REL=StyleSheet HREF='stw.css'><body><center><div class='stw_o_d'><table id='stw_o_t'>";
$footer = "</table></div></body></html>";
//combine the strings
$html = $header . $tables . $footer;

/*
 * Printing time!
 */

echo $html;

?>
