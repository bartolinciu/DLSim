/**
 * @file gate.h
 * @author Michał Ferenc
 * @date 28.10.2020
 * @brief File containing declaration of gate structure and evaluating function declarations.
 */

#ifndef GATE_H
#define GATE_H

#include <iostream>
#include <memory>
#include <unordered_map>

struct Gate;
typedef bool ( *evaluating_function )( std::shared_ptr< Gate > ); //**<typedef of a pointer to function calculating value of gate

/**	Function elaborates output value for input nodes.
	@param gate pointer to gate to be evaluated.
	@return Function returns logic value of the gate.
*/
bool evaluate_in( std::shared_ptr<Gate> gate );

/**	Function elaborates output value for output nodes.
	@param gate pointer to gate to be evaluated.
	@return Function returns logic value of the gate.
*/
bool evaluate_out( std::shared_ptr<Gate> gate );

/**	Function elaborates output value for negation gates.
	@param gate pointer to gate to be evaluated.
	@return Function returns logic value of the gate.
*/
bool evaluate_neg( std::shared_ptr<Gate> gate );

/**	Function elaborates output value for and gates.
	@param gate pointer to gate to be evaluated.
	@return Function returns logic value of the gate.
*/
bool evaluate_and( std::shared_ptr<Gate> gate );

/**	Function elaborates output value for nand gates.
	@param gate pointer to gate to be evaluated.
	@return Function returns logic value of the gate.
*/
bool evaluate_nand( std::shared_ptr<Gate> gate );

/**	Function elaborates output value for or gates.
	@param gate pointer to gate to be evaluated.
	@return Function returns logic value of the gate.
*/
bool evaluate_or( std::shared_ptr<Gate> gate );

/**	Function elaborates output value for nor gates.
	@param gate pointer to gate to be evaluated.
	@return Function returns logic value of the gate.
*/
bool evaluate_nor( std::shared_ptr<Gate> gate );

/**	Function elaborates output value for xor gates.
	@param gate pointer to gate to be evaluated.
	@return Function returns logic value of the gate.
*/
bool evaluate_xor( std::shared_ptr<Gate> gate );

/**	Function elaborates output value for nor gates.
	@param gate pointer to gate to be evaluated.
	@return Function returns logic value of the gate.
*/
bool evaluate_xnor( std::shared_ptr<Gate> gate );


/**
A structure that represents single logical gate in the circuit with it's connections to other gates.
*/
struct Gate{
	std::shared_ptr< Gate > input1_ptr;/**< Pointer to gate which is the first input*/
	std::shared_ptr< Gate > input2_ptr;/**< Pointer to gate which is the second input*/

	int input1;/**< Number of first input node*/
	int input2;/**< Number of second input node*/
	int output;/**< Number of output node*/

	bool value;/**< Logical value of output*/
	bool evaluated;/**< Flag set if the gate has been evaluated for current inputs*/

	evaluating_function evaluate;/**< Pointer to function which elaborates output value depending on inputs and type of gate*/

	/**	Function creates new instantion of Gate structure according to the input
	@param evaluating_functions unordered map connecting gate names with functions evaluating their values
	@param op Name of the gate.
	@param node1 First node of the gate, depending on type input1 or output.
	@param node2 Second node of the gate, depending on type input2, output or unused.
	@param node3 Third node of the gate, depending on type output or unused.
	@return Function returns the pointer to freshly created instance or NULL if operation fails.
	*/
	static std::shared_ptr< Gate > create( std::unordered_map< std::string, evaluating_function > evaluating_functions, const std::string & op, int node1, int node2 = 0, int node3 = 0 );
};

#endif