#pragma once
#include <string>
#include <set>

#include "Form.h"
#include "VariableMap.h"

class VariableMap;

class Symbol: public Form {
public:
	Symbol (std::string);
	~Symbol();
	std::shared_ptr<Form> evaluate(VariableMap&);
	FormType getType();
	std::string quote();
	bool asBoolean(); 
private:
	std::string name;
};
