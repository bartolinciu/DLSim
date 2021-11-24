#include <string>
#include <unordered_map>

#include "gate.h"

/**	Function elaborates output value for input nodes.
	@param gate pointer to gate to be evaluated.
	@return Function returns logic value of the gate.
*/
bool evaluate_in( std::shared_ptr< Gate > gate ){
	return gate->value;
}

/**	Function elaborates output value for output nodes.
	@param gate pointer to gate to be evaluated.
	@return Function returns logic value of the gate.
*/
bool evaluate_out( std::shared_ptr< Gate > gate ){
	return gate->input1_ptr->evaluate( gate->input1_ptr );
}

/**	Function elaborates output value for negation gates.
	@param gate pointer to gate to be evaluated.
	@return Function returns logic value of the gate.
*/
bool evaluate_neg( std::shared_ptr< Gate > gate ){
	if(gate->evaluated)
		return gate->value;
	gate->value = !gate->input1_ptr->evaluate( gate->input1_ptr );
	gate->evaluated = true;
	return gate->value;
}

/**	Function elaborates output value for and gates.
	@param gate pointer to gate to be evaluated.
	@return Function returns logic value of the gate.
*/
bool evaluate_and( std::shared_ptr< Gate > gate ){
	if(gate->evaluated)
		return gate->value;
	return gate->input1_ptr->evaluate( gate->input1_ptr ) & gate->input2_ptr->evaluate( gate->input2_ptr );
	gate->evaluated = true;
	return gate->value;
}

/**	Function elaborates output value for nand gates.
	@param gate pointer to gate to be evaluated.
	@return Function returns logic value of the gate.
*/
bool evaluate_nand( std::shared_ptr< Gate > gate ){
	if(gate->evaluated)
		return gate->value;
	gate->value = !( gate->input1_ptr->evaluate( gate->input1_ptr ) & gate->input2_ptr->evaluate( gate->input2_ptr ));
	gate->evaluated = true;
	return gate->value;
}

/**	Function elaborates output value for or gates.
	@param gate pointer to gate to be evaluated.
	@return Function returns logic value of the gate.
*/
bool evaluate_or( std::shared_ptr< Gate > gate ){
	if(gate->evaluated)
		return gate->value;
	gate->value = gate->input1_ptr->evaluate( gate->input1_ptr ) | gate->input2_ptr->evaluate( gate->input2_ptr );
	gate->evaluated = true;
	return gate->value;
}

/**	Function elaborates output value for nor gates.
	@param gate pointer to gate to be evaluated.
	@return Function returns logic value of the gate.
*/
bool evaluate_nor( std::shared_ptr< Gate > gate ){
	if(gate->evaluated)
		return gate->value;
	gate->value = !( gate->input1_ptr->evaluate( gate->input1_ptr ) | gate->input2_ptr->evaluate( gate->input2_ptr ));
	gate->evaluated = true;
	return gate->value;
}

/**	Function elaborates output value for xor gates.
	@param gate pointer to gate to be evaluated.
	@return Function returns logic value of the gate.
*/
bool evaluate_xor( std::shared_ptr< Gate > gate ){
	if(gate->evaluated)
		return gate->value;
	gate->value = gate->input1_ptr->evaluate( gate->input1_ptr ) ^ gate->input2_ptr->evaluate( gate->input2_ptr );
	gate->evaluated = true;
	return gate->value;
}

/**	Function elaborates output value for nor gates.
	@param gate pointer to gate to be evaluated.
	@return Function returns logic value of the gate.
*/
bool evaluate_xnor( std::shared_ptr< Gate > gate ){
	if(gate->evaluated)
		return gate->value;
	gate->value = !( gate->input1_ptr->evaluate( gate->input1_ptr ) ^ gate->input2_ptr->evaluate( gate->input2_ptr ));
	gate->evaluated = true;
	return gate->value;
}


/**	Function creates new instantion of Gate structure according to the input
	@param evaluating_functions unordered map connecting gate names with functions evaluating their values
	@param op Name of the gate.
	@param node1 First node of the gate, depending on type input1 or output.
	@param node2 Second node of the gate, depending on type input2, output or unused.
	@param node3 Third node of the gate, depending on type output or unused.
	@return Function returns the pointer to freshly created instance or NULL if operation fails.
	*/
std::shared_ptr< Gate > Gate::create( std::unordered_map< std::string, evaluating_function > evaluating_functions, const std::string & op, int node1, int node2, int node3 ){
	if( evaluating_functions.find( op ) == evaluating_functions.end() ){
		return NULL;
	}
	if( node1 <= 0 )
		return NULL;
	std::shared_ptr< Gate > gate( new Gate );
	gate->evaluate = evaluating_functions[ op ];
	gate->evaluated = false;
	if( op == "IN:" ){
		gate->output = node1;
		gate->input1 = 0;
		gate->input2 = 0;
		return gate;
	}else if( op == "OUT:" ){
		gate->input1 = node1;
		gate->input2 = 0;
		return gate;
	}
	if( node2 <= 0 ){
		return NULL;
	}
	if( op == "NEG" || op == "NOT" ){
		gate->input1 = node1;
		gate->input2 = 0;
		gate->output = node2;
		return gate;
	}
	if( node3 <= 0 ){
		return NULL;
	}
	gate->input1 = node1;
	gate->input2 = node2;
	gate->output = node3;
	return gate;
}
