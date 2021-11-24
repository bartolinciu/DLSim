#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <unordered_map>

#include "circuit.h"
#include "gate.h"


/**
	Generic constructor with no parameters, sets up flags.
*/
Circuit::Circuit(){
	this->_good = true;
	this->built = false;
}

/** Method builds a circuit structure according to the circuit file.
 @return Method returns true if circuit has been built succesfully, otherwise returns false and sets an error message.
  @param fname Name of the file where the circuit structure is stored.
*/ 
int Circuit::build( const std::string & fname ){
	std::ifstream file( fname );
	if( !file ){
		this->errors.push_back( "error: Couldn't open circuit file for reading!" );
		this->_good = false;
		return 1;
	}

	//Initializing map connecting names of the gates to functions evaluating them
	std::unordered_map< std::string, evaluating_function > evaluating_functions; 
	evaluating_functions[ "IN:" ] = evaluate_in;
	evaluating_functions[ "OUT:" ] = evaluate_out;
	evaluating_functions[ "NEG" ] = evaluate_neg;
	evaluating_functions[ "NOT" ] = evaluate_neg;
	evaluating_functions[ "AND" ] = evaluate_and;
	evaluating_functions[ "NAND" ] = evaluate_nand;
	evaluating_functions[ "OR" ] = evaluate_or;
	evaluating_functions[ "NOR" ] = evaluate_nor;
	evaluating_functions[ "XOR" ] = evaluate_xor;
	evaluating_functions[ "XNOR" ] = evaluate_xnor;


	//Reading circuit structure from file
	std::string line;
	int line_no = 0;
	while( getline( file, line ) ){
		line_no++;
		if(line == "\r" || line == "")
			continue;
		std::stringstream stream( line );
		std::string op;
		int node1 = 0;
		int node2 = 0;
		int node3 = 0;
		stream>>op;
		
		if( op == "IN:" ){
			int node = 0;
			while( stream>>node ){
				std::shared_ptr<Gate> gate = Gate::create( evaluating_functions, op, node );
				this->gates[gate->output] = gate;
				this->inputs[gate->output] = gate;
			}
		}else if( op == "OUT:" ){
			int node = 0;
			while( stream>>node ){
				std::shared_ptr<Gate> gate = Gate::create( evaluating_functions, op, node );
				this->outputs.push_back( gate );
			}
		}else{

			stream>>node1>>node2>>node3;
			std::shared_ptr<Gate> gate = Gate::create( evaluating_functions, op, node1, node2, node3 );
			if(gate!=NULL){
				this->gates[gate->output] = gate;			
			}
			else{
				this->errors.push_back( fname + ": line: " + std::to_string(line_no) + " warning: ommiting line - incorrect data" );
				this->_good = false;
			}
		}
		
	}
	file.close();

	//actually building dependencies tree
	for( const auto & i : this->gates ){
		auto gate = i.second;
		if( gate->input1 != 0 ){
			if( this->gates.find( gate->input1 ) == this->gates.end() ){
				this->errors.push_back( fname + ": error: unplugged input node: " + std::to_string(gate->input1) );
				this->_good = false;
			}else{
				gate->input1_ptr = this->gates[ gate->input1 ];
			}
		}
		if( gate->input2 != 0 ){
			if( this->gates.find( gate->input2 ) == this->gates.end() ){
				this->errors.push_back( fname + ": error: unplugged input node: " + std::to_string(gate->input2) );
				this->_good = false;
			}else{
				gate->input2_ptr = this->gates[ gate->input2 ];
			}
		}

	}

	for( const auto & gate : this->outputs ){
		if( this->gates.find( gate->input1 ) == this->gates.end() ){
			this->errors.push_back( fname + ": error: unplugged input node: " + std::to_string(gate->input1) );
			this->_good = false;
		}else{
			gate->input1_ptr = this->gates[ gate->input1 ];
		}
	}
	this->built = true;
	return 0;
}

/** Method reads sets of inputs from the file and puts them into object's internal vector.
	@param fname Name of the file where the inputs are stored.
	@return Method returns true if the operation succeded, otherwise returns false and sets error message.
*/
bool Circuit::readInputs( const std::string & fname ){
	if( !this->built ){
		this->errors.push_back( "error: Circuit not built" );
		this->_good = false;
		return false;
	}
	std::ifstream file( fname );
	if( !file ){
		this->errors.push_back( "error: Couldn't open inputs file for reading!");
		this->_good = false;
		return false;
	}

	//Reading inputs from the file
	std::string line;
	int line_no = 0;
	while( getline( file, line ) ){
		line_no++;
		if(line == "\r" || line == "")
			continue;
		std::map< int, bool > set;
		std::stringstream stream( line );
		std::string input;
		while( stream>>input ){
			auto separator_position = input.find_first_of( ':' ) + 1;
			int node = stoi( input.substr( 0, separator_position ) );
			//Checking if node is an input node
			if( this->inputs.find( node ) == this->inputs.end() ){
				this->errors.push_back( fname + ": line: " + std::to_string( line_no ) + " warning: incorrect input node: " + std::to_string( node )  ); 
				continue;
			}

			bool value = stoi( input.substr( separator_position ) );
			set[ node ] = value;
		}

		//Looking for input nodes not present in set of inputs
		std::vector< int > missing_inputs;
		for( const auto & input : this->inputs ){
			if( set.find( input.first ) == set.end() ){
				missing_inputs.push_back(input.first);
			}
		}

		if( missing_inputs.size() ){
			std::string warning = fname + ": line: " + std::to_string(line_no) + "warning: following outputs have no defined value and will be defaulted to 0: ";
			for( int i : missing_inputs ){
				warning += std::to_string( i ) + " ";
			}
			this->errors.push_back( warning );
		}
		this->input_sets.push_back( set );
	}

	file.close();
	return true;
}
/** Method evaluates output values for each set of input data and stores them in object's internal vector.
@return Method returns true if the operation succeded, otherwise returns false and sets error message.
*/
bool Circuit::evaluate(){
	if( !this->built ){
		this->errors.push_back( "error: Circuit not built" );
		this->_good = false;
		return false;
	}
	for( const auto & set : this->input_sets ){
		for( const auto & gate : this->gates ){
			gate.second->evaluated = false;
		}
		for( const auto & input : set){
			this->gates[input.first]->value = input.second;
		}
		std::map<int, bool> output_set;
		for( const auto & output : this->outputs ){
			output_set[ output->input1 ] = output->evaluate( output );
		}
		this->output_sets.push_back( output_set );
	}

	return true; 
}

/** Method writes evaluated outputs into file.
@param fname Name of the file where the output values are supposed to be stored.
@return Method returns true if the operation succeded, otherwise returns false and sets error message.
*/
bool Circuit::writeOutputs( const std::string & fname ){
	if( !this->built ){
		this->errors.push_back( "error: Circuit not built" );
		this->_good = false;
		return false;
	}
	std::ofstream file( fname );
	if( !file ){
		this->errors.push_back( "error: Couldn't open output file for writing!" );
		this->_good = false;
		return false;
	}
	for( int i = 0; i < this->output_sets.size(); i++ ){
		file<<"IN: ";
		for( const auto & input : this->input_sets[i] ){
			file<<input.first<<":"<<input.second<<" ";
		}
		file<<"OUT: ";
		for( const auto & output : this->output_sets[i] ){
			file<<output.first<<":"<<output.second<<" ";
		}
		file<<std::endl;
	}
	file.close();
	return true;
}

/**
	@return Function returns vector containing descriptions of encountered errors.
*/
std::vector<std::string> Circuit::get_errors(){
	return this->errors;
}

/**
	@return Function returns last encountered error.
*/
std::string Circuit::get_last_error(){
	return this->errors[ this->errors.size() - 1 ];
}

/**
	@return Function returns status of the circuit.
*/
bool Circuit::good(){
	return this->_good;
}

/**
	Function clears error list and sets the state to good.
*/
void Circuit::clear_errors(){
	this->errors.clear();
	this->_good = true;
}
