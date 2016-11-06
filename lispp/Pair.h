#pragma once

#include <string>
#include <memory>
#include <sstream>

#include "Form.h"
#include "FormType.h"
#include "Function.h"

class Form;
class List;

class Pair : public Form {
private:	 
	std::shared_ptr<Form> first;
	std::shared_ptr<Form> second;

public:
	Pair() {}
	Pair(std::shared_ptr<Form>, std::shared_ptr<Form>);
	~Pair() {}
	std::shared_ptr<Form> evaluate(VariableMap&);	
	FormType getType(){return FormType::PairType;}
	std::string quote(); 
	bool asBoolean() {return true;}
	void setCar(std::shared_ptr<Form> car) {first = car;}
	void setCdr(std::shared_ptr<Form> cdr) {second = cdr;}
	std::shared_ptr<Form> car() {return first;}
	std::shared_ptr<Form> cdr() {return second;}
	bool isList();
	std::shared_ptr<List> asList();
}; 
