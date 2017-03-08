<?php

function read_args($argv, $options) {
	if (count($argv) > 1) {
		unset($argv[0]);	// Ignoruji nazev souboru
		foreach ($argv as $parametr) {
			if ($parametr[0] != '-') error("Bad parametr '$parametr'!");

			switch ($parametr) {
				case '--help':
					if (count($argv) != 1) error("Only '--help' parametr is allowed!");

					echo "Help\n";
					exit(0);

				case (preg_match('/^--input=.*/', $parametr) ? true : false):
					if (!$options->input) {
						$options->init_input(substr($parametr, 8));
						if (!$options->input) error("Opening the file '" . substr($parametr, 8) . "' was unsuccesful!");
					}
					else 
						error("You can use only one input!");

					break;

				case (preg_match('/^--output=.*/', $parametr) ? true : false):
					if (!$options->output) {
						$options->init_output(substr($parametr, 9));
						if (!$options->output) error("Opening the file '" . substr($parametr, 9) . "' was unsuccesful!");
					}
					else
						error("You can use only one output!");
					break;

				case '-e':
				case '--no-epsiolon-rules':
					if ($options->determinization) error("You can't use '--no-epsiolon-rules' with '--determinization'!");
					$options->no_epsilon_rules = true;
					break;

				case '-d':
				case '--determinization':
					if ($options->no_epsilon_rules) error("You can't use '--determinization' with '--no-epsiolon-rules'!");
					$options->determinization = true;
					break;

				case '-i':
				case '--case-insensitive':
					$options->case_insensitive = true;
					break;

				case '-w':
				case '--white-char':
					$options->white_char = true;
					break;

				case '-r':
				case '--rules-only':
					$options->rules_only = true;
					break;

				case (preg_match('/^--analyze-string=".*"/', $parametr) ? true : false):
					$options->analyze_string = substr($parametr, 18, strlen($parametr) - 19);
					break;

				case '-wsfa':
					$options->wsfa = true;
					break;

				default:
					error();
			}
		}
	}
	
	if (!$options->input)  $options->init_input();
	if (!$options->output) $options->init_output();
}

?>
