#pragma once

#include <string>
#include <memory>
#include <sstream>

#include "Form.h"
#include "FormType.h"
#include "Function.h"
#include "Pair.h"

class Form; 
class Pair;

class List : public Form {
private:	 
	std::vector<std::shared_ptr<Form> > elements;

public:
	List() {}
	List(std::vector<std::shared_ptr<Form> > _elements) {elements = _elements;}
	~List() {}
	std::shared_ptr<Form> evaluate(VariableMap&);	
	FormType getType(){return FormType::ListType;}
	std::string quote();
	void addElement(std::shared_ptr<Form> arg) {elements.push_back(arg);} 
	std::vector<std::shared_ptr<Form> > getElements() {return elements;}
	bool asBoolean() {return true;}
	std::shared_ptr<Form> getElement(int);
	std::shared_ptr<List> tail(int);
	std::shared_ptr<Pair> asPair();
}; 
