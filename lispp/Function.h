#pragma once
#include <memory>
#include <string>
#include <vector>

#include "Form.h"
#include "Expression.h"

class Form;
class Expression;

class Function : public Form {
private:
	std::vector<std::string> argNames;
	std::shared_ptr<Expression> body;	
	
public:
	Function(std::vector<std::string>&, std::shared_ptr<Expression>);
	~Function() {}
	std::shared_ptr<Form> evaluate(VariableMap& vars);
	FormType getType() {return FormType::FunctionType;}
	std::string quote() {return "go fuck yourself";}
	bool asBoolean() {return true;}
	std::shared_ptr<Form> apply(const std::vector<std::shared_ptr<Form> >&, VariableMap&);
};
