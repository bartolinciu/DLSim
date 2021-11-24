/**
 * @file circuit.h
 * @author Michał Ferenc
 * @date 28.10.2020
 * @brief File containing definition of main circuit class.
 */

#ifndef CIRCUIT_H
#define CIRCUIT_H

#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <vector>

#include "gate.h"

/**
Class representing simulated circuit
It takes care of building the structure of the circuit according to the circuit file, 
reading input values from the inputs file and elaborating and writing output values to output file.
 */
class Circuit
{	
	std::map< int, std::shared_ptr< Gate > > gates;   	//**<Map connecting nubers of output nodes of gates to instances representing those gates
	std::map< int, std::shared_ptr< Gate > > inputs;	//**<Map connecting nubers of input nodes to instances representing those inputs
	std::vector< std::shared_ptr< Gate > > outputs; 	//**<Vector containing pointers to instances of output nodes
	std::vector< std::map< int, bool > > output_sets;	//**<Vector containing sets of input data stored as maps connecting numbers of input nodes to their value
	std::vector< std::map< int, bool > > input_sets;	//**<Vector containing sets of elaborated output data stored as maps connecting numbers of output nodes to their value
	std::vector< std::string > errors;					//**<Vector containing error messages
	bool _good;											//**<Flag set if there are no problems with the circuit
	bool built;											//**<Flag set if the circuit has been built succesfully
	
public:
	/**
		Constructor with no parameters, sets up flags.
	*/
	Circuit();

	/** Method builds a circuit structure according to the circuit file.
		@return Method returns true if circuit has been built succesfully, otherwise returns false and sets an error message.
		@param fname Name of the file where the circuit structure is stored.
	*/ 
	int build( const std::string & fname );

	/** Method reads sets of inputs from the file and puts them into object's internal vector.
		@param fname Name of the file where the inputs are stored.
		@return Method returns true if the operation succeded, otherwise returns false and sets error message.
	*/
	bool readInputs( const std::string & fname );

	/** Method evaluates output values for each set of input data and stores them in object's internal vector.
		@return Method returns true if the operation succeded, otherwise returns false and sets error message.
	*/
	bool evaluate();

	/** Method writes evaluated outputs into file.
		@param fname Name of the file where the output values are supposed to be stored.
		@return Method returns true if the operation succeded, otherwise returns false and sets error message.
	*/
	bool writeOutputs( const std::string & fname );

	/**
		@return Function returns status of the circuit.
	*/
	bool good();

	/**
		@return Function returns last encountered error.
	*/
	std::string get_last_error();

	/**
		@return Function returns vector containing descriptions of encountered errors.
	*/
	std::vector< std::string > get_errors();

	/**
		Function clears error list and sets the state to good.
	*/
	void clear_errors();


};

#endif
