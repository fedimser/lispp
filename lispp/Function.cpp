#include "Function.h"

 
Function::Function(std::vector<std::string>& _argNames, std::shared_ptr<Expression> _body) {
	for (size_t i = 0; i < _argNames.size(); ++i)  {
		for (size_t j = i + 1; j < _argNames.size(); ++j)  {
			if (argNames[i] == _argNames[j]) throw std::string("syntax error");
		}
	} 		

	//argNames = std::move(_argNames);
	body = _body;
}
	  
 
	
// First element of arguments is ignored
std::shared_ptr<Form> Function::apply(const std::vector<std::shared_ptr<Form> >& arguments, VariableMap& vars) {
	int argCount = (int)arguments.size() - 1;
	if (argCount != (int)argNames.size()) throw std::string("runtime error");
	for (int i = 0; i < argCount; ++i) {
		vars.pushValue(argNames[i], arguments[i+1]);
	}
	
	std::shared_ptr<Form> ret = body->evaluate(vars);
	
	for (int i = 0; i < argCount; ++i) {
		vars.popValue(argNames[i]);
	}
		
	return ret;
}    

std::shared_ptr<Form> Function::evaluate(VariableMap& vars) {
	vars.doNothing();
	return std::shared_ptr<Form>(this);
}
