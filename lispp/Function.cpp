#include "Function.h"

Function::Function(std::vector<std::string>& _argNames,
				   std::shared_ptr<Form> _body) {
	for (size_t i = 0; i < _argNames.size(); ++i)  {
		for (size_t j = i + 1; j < _argNames.size(); ++j)  {
			if (_argNames[i] == _argNames[j]) throw std::string("syntax error");
		}
	} 		

	argNames = _argNames;
	body = _body;
}
	  
  
std::shared_ptr<Form> Function::evaluate(VariableMap& vars) {
	vars.doNothing();
	
	//self-evaluating
	return std::static_pointer_cast<Form>(shared_from_this());
}

std::shared_ptr<Form> Function::apply(std::vector<std::shared_ptr<Form> >& argValues, VariableMap& vars) {
	
	if (argValues.size() != argNames.size()) throw std::string("runtime error");
	int argCount = argNames.size();
	
	for (int i = 0; i < argCount; ++i) {
		vars.pushValue(argNames[i], argValues[i]);
	}
	
	for (auto var : context) {
		vars.pushValue(var.first, var.second);
	} 
	 
	std::shared_ptr<Form> ret = body->evaluate(vars); 
	
	for (auto var : context) { 
		context[var.first] = vars.getValue(var.first);
		vars.popValue(var.first);
	} 
	
	for (int i = 0; i < argCount; ++i) {
		argValues[i] = vars.getValue(argNames[i]);
		vars.popValue(argNames[i]);
	}
	return ret;
}
std::string Function::quote() {
	std::stringstream ret;
	int argCount = argNames.size();
	ret << "#<CLOSURE <anon> (";
	for(int i = 0; i < argCount; ++i) {
		ret << argNames[i];
		if (i < (int) argValues.size()) ret << "=" << argValues[i]->quote();
		if (i != argCount-1) ret << " ";
	}
	ret << ")" << body->quote() << ">";
	
	/*
	if (!context.empty()){
		ret <<"\nCONTEXT:";
		for(auto it: context)
		ret << it.first <<"="<<it.second->quote()<<",";
	}*/
	
	return ret.str();
}
