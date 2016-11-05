#pragma once
#include <string>
#include <memory>
#include <map>
#include <stack>

#include "Form.h"

class Form;

class VariableMap {
private:
	std::map<std::string, std::stack<std::shared_ptr<Form> > > vars;

public:
	void setValue(std::string name, std::shared_ptr<Form> value);	
	void pushValue(std::string name, std::shared_ptr<Form> value);	
	void popValue(std::string);
	std::shared_ptr<Form> getValue(std::string);
	void doNothing();
};
