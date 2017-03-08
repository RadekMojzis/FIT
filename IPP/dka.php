#!/usr/bin/env php5

<?php
$rm_e_flag = false;
$deter_flag = false;
$insensitive = false;
$output = STDOUT;
$input_file = STDIN;

function validate_fsm($fsm){
  $states = $fsm['states'];
  $alphabet = $fsm['alphabet'];
  $rules = $fsm['rules'];
  $initial_state = $fsm['initial_state'];
  $final_states = $fsm['final_states'];
  
  if(count($alphabet) == 0){
    fwrite(STDERR, "Error, alphabet must not be empty!\n");
    //print_fsm($fsm);  
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
    if($state == "")
      continue;
    if(array_search($state, $states )===false){
      fwrite(STDERR, "Error - final states - undefined state: \"".$state."\"!\n");
    
      exit(41);
    }
  }
}

function determinize_fsm($fsm){
  $states = $fsm['states'];
  $alphabet = $fsm['alphabet'];
  $rules = $fsm['rules'];
  $initial_state = $fsm['initial_state'];
  $final_states = $fsm['final_states'];
  
  $initial_state_deter = $initial_state;
  $states_deter = array($initial_state);
  $states_new = array(array($initial_state_deter));
  $rules_deter = array();
  $final_states_deter = array();

  while(count($states_new)){
    $current_new = array_shift($states_new);
		foreach ($alphabet as $character) {
      $final = false;
      $tmp_state = array ();
      foreach($states as $state){
        foreach($rules as $rule){
          if ($rule['character'] == $character && $rule['starting_state'] == $state && !in_array($rule['final_state'], $tmp_state)) {
						array_push($tmp_state, $rule['final_state']);
						if (!$final)
							$final = in_array($rule['final_state'], $final_states);
					}
        }
      }
      
      if(!empty($tmp_state)) {
				sort($tmp_state);
				$is_state = in_array(implode("_", $tmp_state), $states_deter); 

				$new_final = implode("_", $tmp_state);
				if (!$is_state) {
					array_push($states_new, $tmp_state);
          array_push($states_deter, $new_final);
          if($final){
            array_push($final_states_deter, $new_final);
          }
				}
	
        array_push($rules_deter, array('starting_state' => implode("_",$current_new),'character' => $character ,'final_state' =>$new_final));
			}	
    }
    //var_dump($states_new);
  }
  
  return array('states' => $states_deter, 
               'alphabet' => $alphabet, 
               'rules' => $rules_deter, 
               'initial_state' => $initial_state_deter, 
               'final_states' => $final_states_deter);
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
  $states = $fsm['states'];
  $alphabet = $fsm['alphabet'];
  $rules = $fsm['rules'];
  $initial_state = $fsm['initial_state'];
  $final_states = $fsm['final_states'];
  
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
  return array('states' => $states, 'alphabet' => $alphabet, 'rules' => $new_rules, 'initial_state' => $initial_state, 'final_states' => $new_final_states);
}

function print_fsm($fsm){
  $states = $fsm['states'];
  $alphabet = $fsm['alphabet'];
  $rules = $fsm['rules'];
  $initial_state = $fsm['initial_state'];
  $final_states = $fsm['final_states'];
  $out = $GLOBALS['output'];
	
  fprintf($out, "(\n{");
  foreach($states as $state){
    fprintf($out, "%s", $state);
    if($state != $states[count($states)-1]){
      fprintf($out, ", ");
    }
  }
  fprintf($out, "},\n{");
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
    fwrite(STDERR, "invalid pattern!\n");
    exit(41);
  }

  if(preg_match_all("/(?<={).*(?=},)|(?<=,{).*(?=})/sU", $soubor, $substr_arr) == 0){
    var_dump($substr_arr);
    fwrite(STDERR, "something went wrong :(!\n");
    exit(41);
  }
  if(count($substr_arr[0]) == 5){
    $substr_arr[0][3] = "";
    array_pop($substr_arr[0]);
  }
  
  if(count($substr_arr[0])!= 4){
    var_dump($substr_arr);
    exit(41);
  }
  
  preg_match("/(?<=},)\w+(?=,{)/U", $soubor, $initial_state_arr);
  $states_str = $substr_arr[0][0];
  $alphabet_str = $substr_arr[0][1];
  $rules_str = $substr_arr[0][2];
  $final_states_str = $substr_arr[0][3];
  
  preg_match_all ("/((?<=,\').(?=\',))|((?<=^\').(?=\',))|((?<=,\').(?=\'$))|((?<=^\').(?=\'$))/suU" , $alphabet_str, $alphabet_arr);
  $states = explode("," , $states_str);
  $alphabet = $alphabet_arr[0];
  $rules = array();
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
    
  return array('states' => $states,
               'alphabet' => $alphabet, 
               'rules' => $rules, 
               'initial_state' => $initial_state, 
               'final_states' => $final_states);
}

function load_input(){
  $input = "";
  while(($line = fgets($GLOBALS['input_file'])) != false){
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
 	  if(($GLOBALS['input_file'] = fopen($options["input"], 'r')) == false){
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

if($deter_flag){
	$fsm = remove_eps($fsm);
  $fsm = determinize_fsm($fsm);
}

print_fsm($fsm);

//var_dump($rules_arr);