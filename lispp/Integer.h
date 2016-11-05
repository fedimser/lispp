#pragma once
#include <string>
#include <memory>

#include "Form.h"


class Integer: public  Form {
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
