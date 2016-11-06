#pragma once
#include <string>
#include <memory>

#include "Form.h"
#include "VariableMap.h"


class Integer: public Form, public std::enable_shared_from_this<Integer> {
public:
	Integer (int64_t _value);
	~Integer();
	std::shared_ptr<Form> evaluate(VariableMap&);
	FormType getType();	
	std::string quote();
	int64_t getValue();
	bool asBoolean();
private:
	int64_t value;
};
