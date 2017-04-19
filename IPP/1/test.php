#!/usr/bin/env php5
<?php
$stdin = fopen('php://stdin', 'r');
$soubor = "";
while(($line = fgets(STDIN)) != false){
  $soubor = $soubor.$line;
}
echo $soubor."\n";
echo "parsed:\n";

$soubor = ereg_replace("[[:space:]]","", $soubor);
$automat = split("},", $soubor);

$fin = split(",{", $automat[3]);
$automat[3] = $fin[0];
$automat[4] = $fin[1];

$automat[0] = ereg_replace("\({", "", $automat[0]);
$automat[1] = ereg_replace("^{", "", $automat[1]);
$automat[2] = ereg_replace("^{", "", $automat[2]);
$automat[4] = ereg_replace("}\)", "", $automat[4]);

$fsm_states = explode(",", $automat[0]);
$fsm_alphabet = explode(",", $automat[1]);
$fsm_rules = explode(",", $automat[2]);
$fsm_start = $automat[3];
$fsm_fin = explode(",", $automat[4]);

echo "(\n";
echo "{";
foreach($fsm_states as $state){
	echo $state;
	if($state != end($fsm_states)){
		echo ", ";
	}
}
echo "},\n{";
foreach($fsm_alphabet as $letter){
	echo $letter;
	if($letter != end($fsm_alphabet)){
		echo ", ";
	}
}
echo "},\n{\n";
foreach($fsm_rules as $rule){
	echo $rule;
	if($rule != end($fsm_rules)){
		echo ",\n";
	}
}
echo "\n},\n";
echo $fsm_start.",\n{";
foreach($fsm_fin as $finn){
	echo $finn;
	if($finn != end($fsm_fin)){
		echo ",";
	}
}
echo "},\n";
echo ")\n";






