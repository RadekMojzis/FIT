#!/usr/bin/env php5

<?php

$soubor = file_get_contents("input.txt");
$soubor = preg_replace("/(?<!\')#(?!\').*/","", $soubor);
echo $soubor."\n\n";

$soubor = preg_replace("/((?<!\')\s)|(\s(?!\'))/","", $soubor);

$soubor = ereg_replace("''''", "'''", $soubor);


if(preg_match("/({.*},{.*},{.*},\w+,{.*})/U", $soubor )==0){
	echo "Invalid format!\n";
	exit(42);
}

if(preg_match_all("/(?<={).+(?=},)|(?<=,{).+(?=})/U", $soubor, $substr_arr) == 0){
  echo "No match found :/ \n";
}

if(count($substr_arr[0])!= 4){
	echo "Invalid number of components!\n";
  exit(42);
}

preg_match("/(?<=},)\w+(?=,{)/U", $soubor, $starting_state_arr);
	
$states_str = $substr_arr[0][0];
$alphabet_str = $substr_arr[0][1];
$rules_str = $substr_arr[0][2];
$end_states_str = $substr_arr[0][3];

echo "States: ".$states_str."\n";
echo "Alphabet: ".$alphabet_str."\n";
echo "Rules: ".$rules_str."\n";
echo "End states: ".$end_states_str."\n";
echo "Starting state: ".$starting_state_arr[0]."\n";
echo "\nParsed:\n";

$states = explode("," , $states_str);

preg_match_all ("/((?<=,\').+(?=\',))|((?<=^\').+(?=\',))|((?<=,\').+(?=\'$))/U" , $alphabet_str, $alphabet_arr);
//alphabet_arr[0] - pole abecedy
$end_states = explode(",", $end_states_str);
$starting_state = $starting_state_arr[0];

preg_match_all("/((?<=,)\w+\'.+\'->\w+(?=,))|((?<=,)\w+\'.+\'->\w+(?=$))|((?<=^)\w+\'.+\'->\w+(?=,))|((?<=,)\w+\'\'->\w+(?=,))|((?<=^)\w+\'.+\'->\w+(?=,))|((?<=,)\w+\'.+\'->\w+(?=$))/U", $rules_str, $rules_arr_setup);
$i = 0;
foreach($rules_arr_setup[0] as $rule){
	preg_match("/\w+(?=\')/", $rule, $first);
	preg_match("/(?<=\').+(?=\')|(?<=\')(?=\')/U", $rule, $character);
  preg_match("/(?<=->)\w+/", $rule, $final);
	$rules_arr[$i] = array('startig_state' => $first[0], 'character' => $character[0], 'final_state' => $final[0]);
	$i++; 
}
echo "States: ";
foreach($states as $state){
  echo $state.", ";
}
echo "\n";
echo "Alphabet: ";
foreach($alphabet_arr[0] as $character){
	//if($character == "\t"){
	//  echo "\t";
	//}
	//if($character == " "){
	//  echo " ";
	//}
  echo $character.", ";
}
echo "\n";
echo "Rules: \n";

foreach($rules_arr as $rule){
  echo "\t".$rule["startig_state"]."'".$rule["character"]."'->".$rule["final_state"]."\n";
}
echo "Starting state: ". $starting_state."\n";
echo "Final states: ";

foreach($end_states as $state){
  echo $state.", ";
}

//var_dump($rules_arr);
echo "\n";