#pragma once
#include <memory>
#include <string>

#include "Form.h"
#include "VariableMap.h"


class Boolean: public Form, public std::enable_shared_from_this<Boolean> {
private:
	bool value;
public: 
	Boolean(bool _value) {value = _value;}
	std::shared_ptr<Form> evaluate(VariableMap&);
	FormType getType(){return FormType::BooleanType;} 
	std::string quote();
	bool asBoolean() {return value;}
};
