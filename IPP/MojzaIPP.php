#!/usr/bin/env php5

<?php
$rm_e_flag = false;
$deter_flag = false;
$insensitive = false;
$output = STDOUT;

function validate_fsm($fsm){
  $states = $fsm[0];
  $alphabet = $fsm[1];
  $rules = $fsm[2];
  $initial_state = $fsm[3];
  $final_states = $fsm[4];
  
  if(count($alphabet) == 0){
    fwrite(STDERR, "Error, alphabet cannot be empty!\n");
    exit(41);
  }
  if(count(array_unique($states)) != count($states)){
    $states = array_unique($states);
  } 
  if(count(array_unique($alphabet)) != count($alphabet)){
    $alphabet = array_unique($alphabet);
  } 
  if(count(array_unique($rules, SORT_REGULAR)) != count($rules)){
    $rules = array_unique($rules);
  } 
  if(count(array_unique($final_states)) != count($final_states)){
    $final_states = array_unique($final_states);
  }
  foreach($rules as $rule){
    if(array_search($rule['starting_state'], $states)===false){
      fwrite(STDERR, "Error - rules - undefined state: \"".$rule['starting_state']."\"!\n");
      exit(41);
    }
    if(array_search($rule['character'], $alphabet) === false && $rule['character'] != ""){
      fwrite(STDERR, "Error - rules - undefined character: '".$rule['character']."'!\n");
      exit(41);
    }
    if(array_search($rule['final_state'], $states)===false){
      fwrite(STDERR, "Error - rules - undefined state: \"".$rule['starting_state']."\"!\n");
      exit(41);
    }
  }
  if(array_search($initial_state, $states)===false){
    fwrite(STDERR, "Error - undefined initial state: \"".$initial_state."\"!\n");
    exit(41);
  }
  foreach($final_states as $state){
    if(array_search($state, $states )===false){
      fwrite(STDERR, "Error - final states - undefined state: \"".$state."\"!\n");
      exit(41);
    }
  }
}

function determinize_fsm($fsm){
  $states = $fsm[0];
  $alphabet = $fsm[1];
  $rules = $fsm[2];
  $initial_state = $fsm[3];
  $final_states = $fsm[4];
  
  $initial_state_deter = $initial_state;
  $states_deter = array();
  $states_new = array(array($initial_state_deter));
  $rules_deter = array();
  $final_states_deter = array();
  $states_analysed = array(array());
  do{
    $current_new = $states_new[0];
		$new_state = array();
    sort($current_new);
    $current_new_string = implode("_", $current_new);
    if(array_search($current_new_string, $states_deter) === false){
		  array_push($states_deter, $current_new_string);
		}
		
    foreach($current_new as $new_member){
      foreach($alphabet as $character){
			  $changeflag = 0;
				foreach($rules as $rule){
					if($rule['starting_state'] == $new_member && $rule['character'] == $character){
						if(array_search($rule['final_state'], $new_state) === false){
							array_push($new_state, $rule['final_state']);

							$changeflag = 1;
						}
					}
				}
				// add state
				if($changeflag){
					$new_state_string = implode("_", $new_state);
					if(array_search( $new_state, $states_analysed) === false){
						array_push($states_analysed, $new_state);
						array_push($states_new, $new_state);
					}
					if(array_search(array('starting_state' => $current_new_string, 'character' => $character, 'final_state' => $new_state_string), $rules_deter) === false){
						array_push($rules_deter, array('starting_state' => $current_new_string, 'character' => $character, 'final_state' => $new_state_string));
					}
				}
			}
    }
    if(array_intersect($current_new, $final_states) != array()){
			if(array_search($current_new_string, $final_states_deter) === false){
				array_push($final_states_deter , $current_new_string);
			}
    }
		array_shift($states_new);
  }while(count($states_new) != 0);
  
  return array($states_deter, $alphabet, $rules_deter, $initial_state_deter, $final_states_deter);
}


function epsilon_closure($state, $rules){
  $closure_array = array($state);
  do{
    $closure_size = count($closure_array);
    foreach($rules as $rule){
      foreach( $closure_array as $closure_member){
        if($rule['starting_state'] == $closure_member){
          if($rule['character'] == ""){
            array_push($closure_array, $rule['final_state']);
          }
        }
      }
    }
    $closure_array = array_unique($closure_array);
  }while($closure_size != count($closure_array));
  return $closure_array;
}

function remove_eps($fsm){
  $states = $fsm[0];
  $alphabet = $fsm[1];
  $rules = $fsm[2];
  $initial_state = $fsm[3];
  $final_states = $fsm[4];
  
  $new_rules = array();
  foreach($states as $state){
    foreach(epsilon_closure($state, $rules) as $closure_member){
      foreach($rules as $rule){
        if($closure_member == $rule['starting_state'] && $rule['character'] != ""){
          array_push($new_rules, array('starting_state' => $state, 'character' => $rule['character'] , 'final_state' => $rule['final_state']));
        }
      }
    }
  }
  $new_final_states = array();
  foreach($states as $state){
    if(count(array_intersect(epsilon_closure($state, $rules), $final_states))){
      array_push($new_final_states, $state);
    }
  }
  return array($states, $alphabet, $new_rules, $initial_state, $new_final_states);
}

function print_fsm($fsm){
  $states = $fsm[0];
  $alphabet = $fsm[1];
  $rules = $fsm[2];
  $initial_state = $fsm[3];
  $final_states = $fsm[4];
  $out = $GLOBALS['output'];
	
  fprintf($out, "(\n{");
  foreach($states as $state){
    fprintf($out, "%s", $state);
    if($state != $states[count($states)-1]){
      fprintf($out, ", ");
    }
  }
  fprintf($out, "},\n{)");
  foreach($alphabet as $character){
    if($character == "'"){$character = "''";}
    fprintf($out, "'%s'", $character);
    if($character != $alphabet[count($alphabet)-1]){
      fprintf($out, ", ");
    }
  }
  fprintf($out, "},\n{\n");
  foreach($rules as $rule){
    if($rule["character"] == "'"){$rule["character"] = "''";}
    fprintf($out, "%s '%s' -> %s\n", $rule["starting_state"], $rule["character"], $rule["final_state"]);
  }
  fprintf($out, "},\n%s, \n{", $initial_state);

  foreach($final_states as $state){
    fprintf($out, "%s", $state);
    if($state != $final_states[count($final_states)-1]){
      fprintf($out, ", ");
    }
  }
  fprintf($out, "}\n)\n");
}

function parse_input($soubor){
  if(preg_match("/({.*},{.*},{.*},\w+,{.*})/sU", $soubor )==0){
    exit(42);
  }

  if(preg_match_all("/(?<={).+(?=},)|(?<=,{).+(?=})/sU", $soubor, $substr_arr) == 0){
    exit(42);
  }

  if(count($substr_arr[0])!= 4){
    exit(42);
  }
  
  preg_match("/(?<=},)\w+(?=,{)/U", $soubor, $initial_state_arr);
  $states_str = $substr_arr[0][0];
  $alphabet_str = $substr_arr[0][1];
  $rules_str = $substr_arr[0][2];
  $final_states_str = $substr_arr[0][3];

  preg_match_all ("/((?<=,\').(?=\',))|((?<=^\').(?=\',))|((?<=,\').(?=\'$))/suU" , $alphabet_str, $alphabet_arr);
  $states = explode("," , $states_str);
  $alphabet = $alphabet_arr[0];
  
  preg_match_all("/((?<=,)\w+\'.?\'->\w+(?=,))|((?<=,)\w+\'.?\'->\w+(?=$))|((?<=^)\w+\'.?\'->\w+(?=,))/suU", $rules_str, $rules_arr_setup);
  
  $i = 0;
  foreach($rules_arr_setup[0] as $rule){
    preg_match("/\w+(?=\')/", $rule, $first);
    preg_match("/(?<=\').(?=\')|(?<=\')(?=\')/suU", $rule, $character);
    preg_match("/(?<=->)\w+/", $rule, $final);
    $rules[$i] = array('starting_state' => $first[0], 'character' => $character[0], 'final_state' => $final[0]);
    $i++; 
  }
  $initial_state = $initial_state_arr[0];
  $final_states = explode(",", $final_states_str);
  
  sort($states);
  sort($alphabet);
  array_multisort(($rules));
  sort($final_states);
    
  return array($states, $alphabet, $rules, $initial_state, $final_states);
}

function load_input(){
  $input = "";
  while(($line = fgets(STDIN)) != false){
    $input .= preg_replace("/(?<!\')#(?!\').*/","", $line);    # replaces comments with empty string
  }

  $input = preg_replace("/[\r\n]+/", "\n", $input);

  $input = preg_replace("/((?<!\')\s)|(\s(?!\'))/","", $input);

  $input = ereg_replace("''''", "'''", $input);

  if($GLOBALS['insensitive']){
    $input = strtolower ( $input );
  }
  return $input;
}

function get_arguments(){
  $shortopts = "edi";

  $longopts  = array(
    "help",
    "input:",
    "output:",
    "no-epsilon-rules",
    "determinization",
    "case-insensitive",
  );
  $options = getopt($shortopts, $longopts);
  //var_dump($options);
  if((isset($options["e"]) || isset($options["no-epsilon-rules"])) 
     && (isset($options["d"])|| isset($options["determinization"]))){
    fwrite(STDERR, "Invalid combination of starting arguments!.\n");
    exit(1);
  }
  if(isset($options["e"]) || isset($options["no-epsilon-rules"])){
    $GLOBALS['rm_e_flag'] = true;
  }
  if(isset($options["d"]) || isset($options["determinization"])){
    $GLOBALS['deter_flag'] = true;
  }
  if(isset($options["i"]) || isset($options["case-insensitive"])){
    $GLOBALS['insensitive'] = true;
  }
  if(isset($options["help"])){
    if(count($options) != 1){
      fwrite(STDERR, "Invalid combination of starting arguments!.\n");
      exit(1);
    }
    fwrite(STDERR,"Tento skript bude pracovat s těmito parametry:
  --help Vypíše nápovědu.
  --input=filename zadaný vstupní textový soubor v UTF-8 s popisem konečného automatu.
  --output=filename textový výstupní soubor (opět v UTF-8) s popisem výsledného ekvivalentního konečného automatu v předepsaném formátu.
  -e, --no-epsilon-rules pro pouhé odstranění ε-pravidel vstupního konečného automatu.
Parametr nelze kombinovat s parametrem -d (resp. --determinization).
  -d, --determinization provede determinizaci bez generování nedostupných stavů. Parametr nelze kombinovat s parametrem -e (resp. --no-epsilon-rules).
  -i, --case-insensitive nebude brán ohled na velikost znaků při porovnávání symbolů či stavů (tj. a = A, ahoj = AhOj nebo A b = a B); ve výstupu potom budou všechna velká písmena převedena na malá.
Pokud nebude uveden parametr -e ani -d, tak dojde pouze k validaci a normalizovanému výpisu načteného konečného automatu.\n");
     exit(0);  
  }
  if(isset($options["input"])){
    fclose(STDIN);
 	  if(($STDIN = fopen($options["input"], 'r')) == false){
	  	 exit(2);
	  }
  }
  if(isset($options["output"])){
		if(($GLOBALS['output'] = fopen($options["output"], 'w')) === false){
      exit(2);
    }
  }
}

get_arguments();

$soubor = load_input();

$fsm = parse_input($soubor);

validate_fsm($fsm);

if($rm_e_flag){
  $fsm = remove_eps($fsm);
}
//print_fsm($fsm);
if($deter_flag){
	$fsm = remove_eps($fsm);
  $fsm = determinize_fsm($fsm);
}

print_fsm($fsm);

//var_dump($rules_arr);