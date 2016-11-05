#pragma once

#include <string>
#include <memory>
#include <sstream>

#include "Form.h"
#include "FormType.h"
#include "Function.h"

class Form;

class List : public Form {
private:	 
	std::vector<std::shared_ptr<Form> > elements;
	std::string name;

public:
	List() {}
	~List() {}
	std::shared_ptr<Form> evaluate(VariableMap&);	
	FormType getType(){return FormType::ListType;}
	std::string quote();
	void addElement(std::shared_ptr<Form> arg) {elements.push_back(arg);} 
	std::vector<std::shared_ptr<Form> > getElements() {return elements;}
	bool asBoolean() {return true;}
};

#include "List.cpp"
