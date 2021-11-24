#include <filesystem>
#include <iostream>

/**
	Function parses command line parameters and figures out input and output files
	@param argv array of c strings containing command line parameters
	@param argc length of argv 
	@param [in,out] input_file name of file containing inputs
	@param [in,out] circuit_file name of file containing circuit structure
	@param [in,out] output_file name of file containing outputs
*/
std::string parse_options( int argc, char** argv, std::string & input_file,  
	std::string & circuit_file, std::string & output_file ){
	
	const std::string input_switch = "-i";
	const std::string output_switch = "-o";
	const std::string circuit_switch = "-u";
	const std::string help_switch = "-h";
	const std::string help_switch_long = "--help";
	std::string executable = std::filesystem::path( argv[0] ).stem();

	std::string usage = "Usage: " + executable + " [options]";
	const std::string help = "Available options are:\n\t" + 
	circuit_switch + " <file>\tRead circuit structure from <file>\n\t" +
	input_switch + " <file>\tRead inputs from <file>\n\t" + 
	output_switch + "<file> 	Place the outputs into <file>\n\t" +
	help_switch + "," + help_switch_long + "\tDisplay this message";
	
	std::string more_info = "Try " + executable + " --help for more information";
	

	if( argc == 1 ){
		return usage + "\n" + more_info + "\n";
	}

	for( int i = 1; i < argc; i++ ){
		if( argv[i][0] == '-' )
        {
			std::string sw( argv[i] );
			if(sw == help_switch || sw == help_switch_long){
				return usage + "\n" + help + "\n";
				
			}else{
				if( sw != input_switch && sw != circuit_switch && sw != output_switch ){
					return "error: unrecognized command line option: '" + sw +"'\n"  + more_info + "\n";
				}					
				if( i == argc - 1 ){
					return "error: missing filename after: '" + sw + "'\n" + more_info + "\n";
				}
				std::string param(argv[i+1]);
				if( sw == input_switch ){
					input_file = param;
				}
				else if( sw == circuit_switch ){
					circuit_file = param;
				}else if( sw == output_switch ){
					output_file = param;
				}
				i++;
			}
		}
		else{
			return "error: unrecognized parameter: '" + std::string( argv[i] ) + "'\n" + more_info + "\n";
		}
	}

	if( input_file == "" ){
		return "error: File with inputs not specified\n" + more_info + "\n";
	}
	if( circuit_file == "" ){
		return "error: File with circuit not specified\n" + more_info + "\n";
	}
	if( output_file == "" ){
		return "error: File with outputs not specified\n" + more_info + "\n";
	}

	return "";
}
