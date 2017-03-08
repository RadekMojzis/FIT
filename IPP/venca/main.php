<?php
	//FIXME Chybova hlaseni

	// Include source codes
	include('classes.php');
	include('functions.php');
	include('read_args.php');

	// Make objekt of options
	$options = new options();
	// Read args
	read_args($argv, $options);

	// Make finite automaton
	$finite_automaton = new finite_automaton();

	// Nactu prvni (
	if ($options->get_char() != '(')		// getchar != get_char !!!
		error("Finite automaton must star by '('!");

	// Prectu veschny stavy
	read_states($options, $finite_automaton);
	// Nactu vstupni abecedu
	read_alphabet($options, $finite_automaton);
	// Nactu pravidla
	read_rules($options, $finite_automaton);
	// Nactu startovni stav
	read_start_state($options, $finite_automaton);
	// Nactu koncove stavy
	read_final_states($options, $finite_automaton);
// echo $finite_automaton;
	// FIXME Opravit detekci "smeti"
		// error("Some other chars after end of finite automaton ($tmp_char)!");

	if ($options->no_epsilon_rules) {
		remove_e($finite_automaton);
	}
	if ($options->determinization) {
		remove_e($finite_automaton);
		determinization($finite_automaton);
	}

	fwrite($options->output, $finite_automaton);
?>