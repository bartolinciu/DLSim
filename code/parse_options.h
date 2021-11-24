/**
 * @file parse_options.h
 * @author Michał Ferenc
 * @date 5.11.2020
 * @brief File containing declaration of function parsing command line parameters
 */

#ifndef PARSE_OPTIONS_H
#define PARSE_OPTIONS_H

/**
	Function parses command line parameters and figures out input and output files
	@param argv array of c strings containing command line parameters
	@param argc length of argv 
	@param [in,out] input_file name of file containing inputs
	@param [in,out] circuit_file name of file containing circuit structure
	@param [in,out] output_file name of file containing outputs
*/
std::string parse_options( int argc, char** argv, std::string & input_file,  
	std::string & circuit_file, std::string & output_file );

#endif