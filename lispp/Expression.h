#pragma once
#include <memory>
#include <vector>
#include <string>

#include "Form.h"
#include "Function.h"
#include "Integer.h"
#include "Boolean.h"
#include "Symbol.h"
#include "List.h"

class Expression : public Form {
private:	
	std::string name;
	std::vector<std::shared_ptr<Form> > arguments;
	
	
	bool compare(std::shared_ptr<Form>, std::shared_ptr<Form>);
	int64_t accumulateInitial();
	int64_t accumulate(std::shared_ptr<Form>, int64_t);
	std::vector<std::shared_ptr<Form> > evaluateAllArguments(VariableMap&);

public:
	Expression(std::string);	
	Expression(){};
	~Expression(){};

	std::shared_ptr<Form> evaluate(VariableMap&);
	FormType getType(){return FormType::ExpressionType;}
	std::string quote();
	void addArgument(std::shared_ptr<Form> arg) {arguments.push_back(arg);} 
	bool asBoolean() {return true;}
};
