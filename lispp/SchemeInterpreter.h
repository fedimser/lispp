#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
 
#include "VariableMap.h"
#include "Form.h"
#include "Integer.h"
#include "Symbol.h"
#include "Boolean.h"
#include "Expression.h"
#include "List.h"

 
class SchemeInterpreter { 
private:
	VariableMap vars;
	bool shouldClose = false;
	
	bool fetchForm(std::string&);
	bool tryMakeInteger(std::string, int64_t&);
	std::string removeExtraSpaces(const std::string&);
	bool isInitial(char);
	bool isSubsequent(char);
	bool isIdentifier(std::string);
	std::shared_ptr<Form> Formize(std::string);
	bool isFunctionName(std::string);
	
public:
	bool debugInfo;
	bool usePrompt;
	
	SchemeInterpreter();  
	void Run();
	std::string processForm(const std::string&);
};
