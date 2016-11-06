#pragma once
#include <memory>
#include <string>
#include <vector>
#include <map>

#include "Form.h"
#include "Expression.h"

class Form;
class Expression;

class Function : public Form, public std::enable_shared_from_this<Function> {
private:
	std::vector<std::string> argNames;
	std::vector<std::shared_ptr<Form> > argValues;
	std::shared_ptr<Form> body;	
	std::map<std::string, std::shared_ptr<Form> >context;
	
public: 
	Function(std::vector<std::string>&, std::shared_ptr<Form>);
	~Function() {}
	std::shared_ptr<Form> evaluate(VariableMap& vars);
	FormType getType() {return FormType::FunctionType;}
	std::string quote();
	bool asBoolean() {return true;}
	std::shared_ptr<Form> apply(std::vector<std::shared_ptr<Form> >&, VariableMap&); 
	void setContext(std::map<std::string, std::shared_ptr<Form> > _context) {context = _context;}
	std::shared_ptr<Form> getBody() {return body;}
};
