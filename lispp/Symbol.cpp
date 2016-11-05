#include "Symbol.h"

Symbol::Symbol (std::string _name) {
	name = _name;
}
	
Symbol::~Symbol(){}
	
std::shared_ptr<Form> Symbol::evaluate(VariableMap& vars) {
	return vars.getValue(name);
}

FormType Symbol::getType(){
	return FormType::SymbolType;
} 

std::string Symbol::quote() {
	return name;
}		 

bool Symbol::asBoolean() {return true;}
