#pragma once

#include <memory>
#include <string>
#include <set>

#include "FormType.h"

class VariableMap;
class Form;

class Form {
public:
	virtual ~Form() {};	
	virtual std::shared_ptr<Form> evaluate(VariableMap&) = 0;
	virtual FormType getType() = 0;
	virtual std::string quote() = 0;
	virtual bool asBoolean() = 0; 
};
