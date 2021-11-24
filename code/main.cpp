/** @mainpage Project Index Page
 *
 * This is simple digital circuit simulator created for the sake of FoCP course.
 *
 */

#include <iostream>


#include "circuit.h"
#include "parse_options.h"



int main( int argc, char **argv ){

	std::string input_file;
	std::string circuit_file;
	std::string output_file;


	std::string parse_error = parse_options( argc, argv, input_file, circuit_file, output_file );
	if( parse_error != "" ){
		std::cout<<parse_error;
		return 0;
	}


	Circuit circuit;
	circuit.build( circuit_file );
	
	if( !circuit.good() ){
		for( const auto & error : circuit.get_errors() ){
			std::cout<<error<<std::endl;
		}
		return 0;
	}
	circuit.readInputs( input_file );
	for( const auto & error : circuit.get_errors() ){
		std::cout<<error<<std::endl;
	}
	if( !circuit.good() ){	
		return 0;
	}
	circuit.evaluate();
	if( !circuit.good() ){
		for( const auto & error : circuit.get_errors() ){
			std::cout<<error<<std::endl;
		}
		return 0;
	}
	circuit.writeOutputs( output_file );
	if( !circuit.good() ){
		for( const auto & error : circuit.get_errors() ){
			std::cout<<error<<std::endl;
		}
		return 0;
	}
	return 0;
} 

