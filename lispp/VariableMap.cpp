#include "VariableMap.h"

void VariableMap::setValue(std::string name, std::shared_ptr<Form> value) {
	if (vars[name].empty()) throw std::string("name error");
	vars[name].pop();
	vars[name].push(value);
}
	
void VariableMap::pushValue(std::string name, std::shared_ptr<Form> value) {
	vars[name].push(value);
}

bool VariableMap::isDefined(std::string name) {
	return (!vars[name].empty());
}
	
void VariableMap::popValue(std::string name) {
	if (vars[name].empty()) throw std::string("runtime error");
	vars[name].pop();
}
	
std::shared_ptr<Form> VariableMap::getValue(std::string name) {
	if (vars[name].empty()) throw std::string("name error");
	return vars[name].top();
}

void VariableMap::doNothing() {}

