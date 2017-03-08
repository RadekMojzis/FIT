<?php 

// Error function
function error ($string = "Some error!", $return_value = 40) {
	error_log("ERROR: $string\n");
	exit($return_value);
}

// Nactu pristi ','
function read_comma (options $options) {
	if ($options->get_char() != ',')
		error("You must use the ',' to divide it!");
}

// Precte vsechny stavy ze vstupu a ulozi je do fa
function read_states (options $options, finite_automaton $fa) {
	// Ctu vstupni stavy
	if ($options->get_char() != '{')		// Vzdy ohraniceny pomoci { }
		error("Set of states must star by '{'!");
	
	$id = '';
	while (($tmp_char = $options->get_char()) != '}') {
		if(($tmp_char >= 'A' && $tmp_char <= 'Z') || ($tmp_char >= 'a' && $tmp_char <= 'z') || ($tmp_char >= '0' && $tmp_char <= '9'))
			$id .= $tmp_char;
		elseif ($tmp_char == '_' && strlen($id)) {
			$id .= $tmp_char;
		}
		elseif ($tmp_char == ',') {
			$fa->add_state(new state($id));
			$id = '';
		}
		else {
			error("Invalid set of states!");
		}
	}
	$fa->add_state(new state($id));

	read_comma($options);
}

// Precte abecedu ze vstupu a ulozi ji do fa
function read_alphabet (options $options, finite_automaton $fa) {
	if ($options->get_char() != '{')		// Vzdy ohraniceny pomoci { }
		error("Set of states must star by '{'!");
	
	while (true) {
		$tmp_char = $options->get_char();
		if ($tmp_char != '\'')
			error("Invalid input alphabet!1 $tmp_char");

		$tmp_char = $options->get_char(TRUE);

		// NOTE Pridat kontrolu na nepovolene znaky ?
		if ($tmp_char == '\'') {
			$tmp_char = $options->get_char(TRUE);
			if ($tmp_char == '\'') {
				$tmp_char = $options->get_char(TRUE);
				if ($tmp_char == '\'') {
					$fa->add_symbol($tmp_char);

					$tmp_char = $options->get_char(TRUE);
					if ($tmp_char == '}')
						break;
					elseif ($tmp_char == ',')
						continue;
				}
					
				error("Invalid input alphabet!2");

			}
			elseif ($tmp_char == '}') {
				$fa->add_symbol('');
				break;
			}
			elseif ($tmp_char == ',') {
				$fa->add_symbol('');
				continue;
			}
			else {
			}
		}
		$fa->add_symbol($tmp_char);
		
		$tmp_char = $options->get_char(TRUE);
		if ($tmp_char != '\'')
			error("Invalid input alphabet!3");

		$tmp_char = $options->get_char();
		if ($tmp_char == '}')
			break;
		elseif ($tmp_char != ',')
			error("Invalid input alphabet!4");
	}

	read_comma($options);
}

// Precte praidla ze vstupu a ulozi je do fa
function read_rules (options $options, finite_automaton $fa) {
	if ($options->get_char() != '{')		// Vzdy ohraniceny pomoci { }
		error("Set of rules must star by '{'!");

	while (($tmp_char = $options->get_char()) != '}') {
		$found_epsilon = FALSE;
		$found_arrow   = FALSE;			

		// Nactu vychozi stav
		$start_state = '';
		while (1) {
			if(($tmp_char >= 'A' && $tmp_char <= 'Z') || ($tmp_char >= 'a' && $tmp_char <= 'z') || ($tmp_char >= '0' && $tmp_char <= '9') || $tmp_char == '_')
				$start_state .= $tmp_char;
			else
				break;
			$tmp_char = $options->get_char();
		}
		// Otestuju ID a ' pro pristi znak
		$start_state = $fa->is_state($start_state);

		if ($tmp_char != '\'' || !$start_state)
			error("Invalid set of rules!1");
		// Nactu a otestuju pismeno abecedy
		$input_char = $options->get_char(TRUE);

		if ($input_char == '\'') {
			$found_epsilon = TRUE;
			$input_char = '';

			$tmp_char = $options->get_char(TRUE);
			if ($tmp_char == '\'') {
				$tmp_char = $options->get_char(TRUE);
				if ($tmp_char == '\'') {
					$input_char = '\'';
				}
			}
			elseif ($tmp_char == '-') {
				if ($options->get_char() != '>')
					error("Invalid set of rules!3");
				$found_arrow = TRUE;			
			}
		}
		if (!$found_epsilon && !$fa->is_input_char($input_char))
			error("Invalid set of rules!4");
		// Nactu ' ->
		if (!$found_epsilon && $options->get_char(TRUE) != '\'')
			error("Invalid set of rules!5");			
		if (!$found_arrow && $options->get_char() != '-')
			error("Invalid set of rules!6");			
		if (!$found_arrow && $options->get_char() != '>')
			error("Invalid set of rules!7");			
		
		// Nactu cilovy stav
		$final_state = '';
		$tmp_char = $options->get_char();
		while (1) {
			if(($tmp_char >= 'A' && $tmp_char <= 'Z') || ($tmp_char >= 'a' && $tmp_char <= 'z') || ($tmp_char >= '0' && $tmp_char <= '9') || $tmp_char == '_')
				$final_state .= $tmp_char;
			else
				break;
			$tmp_char = $options->get_char();
		}

		// Otestuju ID a , pro pristi znak
		$final_state = $fa->is_state($final_state);
		if (!$final_state)
			error("Invalid set of rules!8");
		if ($tmp_char != ',') {
			if ($tmp_char == '}') {
				$fa->add_rule(new rule($start_state, $final_state, $input_char));
				break;
			}
			else {
				error("Invalid set of rules!9");
			}
		}

		$fa->add_rule(new rule($start_state, $final_state, $input_char));
	}

	read_comma($options);
}

// Precte pocatecni stav
function read_start_state (options $options, finite_automaton $fa) {
	$id = '';
	while (($tmp_char = $options->get_char()) != ',') {
		if ($options->is_eof()) {
			error("invalid start states!");
		}
		$id .= $tmp_char;
	}

	if (($tmp_state = $fa->is_state($id))) {
		$fa->set_start_state($tmp_state);
	}
	else
		error("Start state isn't in set of states! - $tmp_state");
}

// Precte koncove stavy
function read_final_states (options $options, finite_automaton $fa) {
	if ($options->get_char() != '{')		// Vzdy ohraniceny pomoci { }
		error("Set of final states must star by '{'!");
	
	$id = '';
	while (($tmp_char = $options->get_char()) != '}') {
		if ($tmp_char == ',') {		// Nacetl jsem id -> porovnam a zaznacim, pokud neexistuje, koncim
			if (($tmp_state = $fa->is_state($id))) {
				$tmp_state->set_final();
				$id = '';
			}
			else
				error("invalid set of final states - state '$id' not exist!1");
		}
		elseif ($options->is_eof()) {
			error("invalid set of final states!");
		}
		else {
			$id .= $tmp_char;
		}
	}
	
	if ($id != '') {	
		$not_found = TRUE;
		foreach ($fa->get_states() as $state) {
			if ($state == $id) {
				$state->set_final();

				$not_found = FALSE;
				break;
			}
		}
		if ($not_found)
			error("invalid set of final states - state '$id' not exist!2");
	}
}



/* * * * * * * * * * *
 * * DKA algoritmy * *
 * * * * * * * * * * */
function remove_e (finite_automaton &$fa) {
	// Vytvorim novy automat
	$new_fa = new finite_automaton();
	foreach ($fa->get_states() as $state)     { $new_fa->add_state($state); }
	foreach ($fa->get_alphabet() as $symbol)  { $new_fa->add_symbol($symbol); }
	$new_fa->set_start_state($fa->get_start_state());

	// Vytvorim e-uzavery
	foreach ($fa->get_states() as $state) {
		$e_closure = array($state);
		// echo "e-uzaver pro '$state' je '$state'\n";
		foreach ($fa->get_rules() as $rule) {
			if ($rule->start_state == $state && empty($rule->input_char) && !array_search($rule->final_state, $e_closure)) {
				if ($rule->final_state->is_final())
					$state->set_final();
				array_push($e_closure, $rule->final_state);
				// echo "e-uzaver pro '$state' je '" . $rule->final_state . "'\n";
			}
		}

		foreach ($e_closure as $closure) {
			foreach ($fa->get_rules() as $rule) {
				if ($rule->start_state == $closure) {
					if (!empty($rule->input_char))
						$new_fa->add_rule(new rule($state, $rule->final_state, $rule->input_char));
					// FIXME test 06: r 'č' ->s ??
				}
			}
		}
	}
	$fa = $new_fa;
}

function determinization (finite_automaton &$fa) {
	// Vytvorim novy automat
	$new_fa = new finite_automaton();
	$new_fa->add_state($fa->get_start_state());
	foreach ($fa->get_alphabet() as $symbol)  { $new_fa->add_symbol($symbol); }
	$new_fa->set_start_state($fa->get_start_state());

	// Postupne vytvorim novy fa

	$workspace = array ($new_fa->get_start_state());
	while ( count($workspace) ) {
		$state = array_shift($workspace);
		
		// echo "# $state\n";
		foreach ($fa->get_alphabet() as $symbol) {
			// echo "\t$symbol:\n";
		
			$is_final = FALSE;
			$tmp_state = array ();
			foreach ($state->get_id() as $id) {

				// echo "\t\t- podstav '$id':\n";
				foreach ($fa->get_rules() as $rule) {
					// echo "\t\t\t.. Testuji $rule ";
					if ($rule->input_char == $symbol && $rule->start_state == $id && !in_array($rule->final_state->__toString(), $tmp_state)) {
						// echo "***\n";
						array_push($tmp_state, $rule->final_state->__toString());
						if (!$is_final)
							$is_final = $fa->is_final_state($rule->final_state);
					}
					// else echo "\n";
				}
				// echo "\t\t\tDostupné stavy: " . implode(", ", $tmp_state) . "\n";
			}

			// Ignoruji prazdny stav
			if(!empty($tmp_state)) {
				natsort($tmp_state);
				$is_state = $new_fa->is_state(implode("_",$tmp_state)); 
				
				// Pokud stav neexistuje -> vytvorim ho
				if (!$is_state) {
					$new_state = new state($tmp_state, $is_final);
					$new_fa->add_state($new_state);
					array_push($workspace, $new_state);
					// echo "\t\t\t# VYTVOŘEN nový stav $new_state ($is_final)\n";
				}
				else
					$new_state = $is_state;
	
				// Vytvorim nove pravidlo
				$new_fa->add_rule(new rule($state, $new_state, $symbol));
				// echo "\t\t\t# VYTVOŘENO nove pravidlo " . new rule($state, $new_state, $symbol) . "\n";
			}	
		}	
	}

	$fa = $new_fa;
}
?>