#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
 
#include <string>
#include <memory>
#include <map>
#include <stack>


#include <memory>
#include <string>
#include <set>


enum FormType {
	IntegerType, 
	BooleanType, 
	FunctionType, 
	SymbolType, 
	ListType,
	PairType,
	ExpressionType
}; 

class VariableMap;
class Form;

class Form {
public:
	virtual ~Form() {};	
	virtual std::shared_ptr<Form> evaluate(VariableMap&) = 0;
	virtual FormType getType() = 0;
	virtual std::string quote() = 0;
	virtual bool asBoolean() = 0; 
};

class Form;

class VariableMap {
private:
	std::map<std::string, std::stack<std::shared_ptr<Form> > > vars;

public:
	void setValue(std::string name, std::shared_ptr<Form> value);	
	void pushValue(std::string name, std::shared_ptr<Form> value);	
	void popValue(std::string);
	std::shared_ptr<Form> getValue(std::string);
	void doNothing();
	bool isDefined(std::string);
};
#include <string>
#include <memory>



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
#include <string>
#include <set>


class VariableMap;

class Symbol: public Form {
public:
	Symbol (std::string);
	~Symbol();
	std::shared_ptr<Form> evaluate(VariableMap&);
	FormType getType();
	std::string quote();
	bool asBoolean(); 
private:
	std::string name;
};
#include <memory>
#include <string>



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
#include <memory>
#include <vector>
#include <string>
#include <vector>
#include <set>

#include <memory>
#include <string>
#include <vector>
#include <map>


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

#include <string>
#include <memory>
#include <sstream>


#include <string>
#include <memory>
#include <sstream>


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

class Expression : public Form {
private:	
	std::string name;
	std::vector<std::shared_ptr<Form> > arguments;
	
	
	bool compare(std::shared_ptr<Form>, std::shared_ptr<Form>); 
	int64_t accumulate(std::shared_ptr<Form>, int64_t);
	std::vector<std::shared_ptr<Form> > evaluateAllArguments(VariableMap&);
	void getAllSymbols(std::shared_ptr<Form>, std::set<std::string>&);
public:
	Expression(std::string);
	Expression(std::string, std::vector<std::shared_ptr<Form> >);	
	Expression(){};
	~Expression(){};

	std::shared_ptr<Form> evaluate(VariableMap&);
	FormType getType(){return FormType::ExpressionType;}
	std::string quote();
	void addArgument(std::shared_ptr<Form> arg) {arguments.push_back(arg);} 
	bool asBoolean() {return true;} 
	std::vector<std::shared_ptr<Form> > getArguments() {return arguments;}
};

 
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
	void bracketCheck(const std::string&);
	
public: 
	bool usePrompt;
	
	SchemeInterpreter();  
	void Run();
	std::string processForm(const std::string&);
};

int main() { 
	
	SchemeInterpreter inter;
	inter.usePrompt = false;
	inter.Run();
	
    return 0;
}

bool SchemeInterpreter::fetchForm(std::string& ret) {
	ret = "";
	char ch;
	int depth = 0;
	bool started=false;
	while(true) {
		if (std::cin.eof()) {
			if (started) {
				std::cout << "syntax error\n";
			}
			return false;
		}
		std::cin.get(ch);
		ret+=ch;
		if (ch == '(') depth++;
		if (ch == ')') depth--;
		if (depth < 0) return true;
		if(ch>' ') started = true;
		if(depth == 0  && started && ch<=' ') return true;
	}
}
	
bool SchemeInterpreter::tryMakeInteger(std::string str, int64_t& res) {
	std::stringstream ss;
	ss<<str;
	if (ss >> res) {
		return true;
	}
	return false;
}
	
std::string SchemeInterpreter::removeExtraSpaces(const std::string& text) {
	int first = 0;
	int last = text.length() - 1;
	while(text[first]<=' ' && first != last) first++;
	if (text[first] <= ' ') return "";
	while(text[last]<=' ')last--;
	return text.substr(first, last-first+1);
}


bool SchemeInterpreter::isInitial(char c) {
	if (c >= 'a' && c <= 'z') return true;
	if (c >= 'A' && c <= 'Z') return true;
	if (c == '!' || c == '$' || c == '%' || c == '&') return true;
	if (c == '*' || c == '/' || c == ':' || c == '<') return true;
	if (c == '=' || c == '>' || c == '?' || c == '~') return true;
	if (c == '_' || c == '^') return true;
	return false;
}

bool SchemeInterpreter::isSubsequent(char c) {
	if (isInitial(c)) return true;
	if (c >= '0' && c<='9') return true;
	if (c == '.' || c == '+' || c == '-') return true;
	return false;
}

bool SchemeInterpreter::isIdentifier(std::string text) {
	if (isFunctionName(text)) return false;
	if (text.length() == 0) return false;
	if (isInitial(text[0])) {
		for (int i = 1; i < (int)text.length(); ++i) {
			if (!isSubsequent(text[i])) return false;
		}
		return true;
	}
	
	if (text == "+" || text == "-") return true;
	return false;
}

void SchemeInterpreter::bracketCheck(const std::string& text) {
	int count = 0;
	for(auto c : text) {
		if (c == '(') ++count;
		if (c == ')') --count;
		if (count < 0) throw std::string("syntax error"); 
	}
	if (count != 0) throw std::string("syntax error"); 
}

std::shared_ptr<Form> SchemeInterpreter::Formize(std::string text) {
	text = removeExtraSpaces(text);
	
	int len = text.length(); 
	if(text[0]!='(' || text[len-1]!=')') {
		int64_t val;
		if (tryMakeInteger(text, val)) {
			 std::shared_ptr<Integer> ret = std::make_shared<Integer>(val);
			 return ret;
		}
		
		if (isIdentifier(text)) {
			return std::make_shared<Symbol>(text);
		}
		
		if (text == "#f") return std::make_shared<Boolean>(false);
		if (text == "#t") return std::make_shared<Boolean>(true);
		
		if (text[0] == '\'') {
			std::shared_ptr<Expression> ret = std::make_shared<Expression>("quote");
			ret->addArgument(Formize(text.substr(1,text.length()-1)));
			return ret;
		}
		
		throw std::string("syntax error");
	}
	
	std::vector<std::string> subForms;
	std::string cur;
	int depth = 0;
	
	for (int i = 1;i<len-1;++i)
	{
		cur += text[i];
		if(text[i]=='(') depth++;
		if(text[i]==')') depth--;
		
		
		bool isEndOfForm = false;
		
		if(depth == 0 && text[i]==' ') isEndOfForm = true;
		if(depth == 0 && text[i]==')') isEndOfForm = true;
		if(depth == 0 && text[i+1]=='(' && text[i]!='\'') isEndOfForm = true; 
		
		if(i==len-2) isEndOfForm = true;
		
		
		if(isEndOfForm && cur.length()!=0) {
			cur = removeExtraSpaces(cur); 
			if (cur.length() != 0) subForms.push_back(cur); 
			cur="";
		}	
	}  
	
	if (subForms.size() != 0 && isFunctionName(subForms[0])) {
		int argsCount = (int)subForms.size() - 1;
		std::shared_ptr<Expression> ret = std::make_shared<Expression>(subForms[0]);
		for (int i = 1; i <= argsCount; ++i) {
			ret->addArgument(Formize(subForms[i]));
		}
		return ret;
	}
	
	//try interpret as dotted pair
	if (subForms.size() >= 3 && subForms[subForms.size() - 2] == ".") {
		std::shared_ptr<Form> left = Formize(subForms[subForms.size() - 3]);
		std::shared_ptr<Form> right = Formize(subForms[subForms.size() - 1]);
		
		std::shared_ptr<Pair> ret = std::make_shared<Pair>(left, right);
		
		for (int i = subForms.size() - 4; i >= 0; --i) {
			ret = std::make_shared<Pair>(Formize(subForms[i]), ret);
		}
		
		return ret;
	}
	 
	//interpret as a list
	std::vector<std::shared_ptr<Form> > elements;
	for (auto str : subForms) {
		elements.push_back(Formize(str));
	} 
	return std::make_shared<List>(elements);
}
	  
bool SchemeInterpreter::isFunctionName(std::string word) {
	if (word == "quote") return true;
	
	if (word == "number?") return true;
	if (word == "boolean?") return true;
	if (word == "symbol?") return true;
	if (word == "list?") return true;
	if (word == "pair?") return true;
	if (word == "null?") return true;
	
	if (word == "+") return true;
	if (word == "-") return true;
	if (word == "*") return true;
	if (word == "/") return true;
	
	if (word == "=") return true;
	if (word == ">") return true;
	if (word == "<") return true;
	if (word == ">=") return true;		
	if (word == "<=") return true;	
	
	if (word == "max") return true;		
	if (word == "min") return true;
	
	if (word == "abs") return true;
	
	if (word == "not") return true;
	if (word == "and") return true;
	if (word == "or") return true;
	
	if (word == "define") return true;		
	if (word == "set!") return true;
	if (word == "lambda") return true;	
	if (word == "begin") return true;
	
	if (word == "if") return true;	
	
	if (word == "car") return true;	
	if (word == "cdr") return true;
	if (word == "set-car!") return true;	
	if (word == "set-cdr!") return true;		
	if (word == "cons") return true;	
	if (word == "list") return true;	
	if (word == "list-ref") return true;	
	if (word == "list-tail") return true;	
	
	
	return false;
}
 

SchemeInterpreter::SchemeInterpreter() {
	usePrompt = false;
}; 	


	
void SchemeInterpreter::Run() {		
	if(usePrompt) {
		std::cout << "*** SCHEME Interpreter by Dmitriy Fedoriaka ***\n>";
	}
	
	std::string form;
	while (fetchForm(form)) {
		std::cout << processForm(form);
		if (shouldClose) break;
		std::cout << std::endl;
		if (usePrompt) std::cout <<">";
	} 
} 


std::string SchemeInterpreter::processForm(const std::string& inputText) {
	try { 
		bracketCheck(inputText);
		std::shared_ptr<Form> form = Formize(inputText); 
		form = form->evaluate(vars);
		std::string ret = form->quote();
		return ret;
	} catch (std::string ex) {
		if (ex == "syntax error") shouldClose = true;
		return ex;
	}
}

Integer::Integer (int64_t _value) {
	value = _value;
}
	
Integer::~Integer(){}
	
std::shared_ptr<Form> Integer::evaluate(VariableMap& vars) {
	vars.doNothing(); 
	return std::static_pointer_cast<Form>(shared_from_this());
}
	
FormType Integer::getType(){
	return FormType::IntegerType;
} 
	
std::string Integer::quote() {
	return std::to_string(value);
}
	
int64_t Integer::getValue() {
	return value;
}
	
bool Integer::asBoolean() {
	return true;
}

void VariableMap::setValue(std::string name, std::shared_ptr<Form> value) {
	if (vars[name].empty()) throw std::string("name error");
	vars[name].pop();
	vars[name].push(value);
}
	
void VariableMap::pushValue(std::string name, std::shared_ptr<Form> value) {
	vars[name].push(value);
}

bool VariableMap::isDefined(std::string name) {
	return (!vars[name].empty());
}
	
void VariableMap::popValue(std::string name) {
	if (vars[name].empty()) throw std::string("runtime error");
	vars[name].pop();
}
	
std::shared_ptr<Form> VariableMap::getValue(std::string name) {
	if (vars[name].empty()) throw std::string("name error");
	return vars[name].top();
}

void VariableMap::doNothing() {}


std::shared_ptr<Form> Boolean::evaluate(VariableMap& vars) {
	vars.doNothing(); 
	return std::static_pointer_cast<Form>(shared_from_this());
}

std::string Boolean::quote() {
	if (value) {
		return "#t";
	} else {
		return "#f";
	}
}

std::shared_ptr<Form> List::evaluate(VariableMap& vars) {
	if (elements.size() != 0) {
		std::shared_ptr<Form> maybeFunc = elements[0]->evaluate(vars);
		if (maybeFunc->getType() == FormType::FunctionType) {
			std::shared_ptr<Function> func = std::static_pointer_cast<Function>(maybeFunc);
			std::vector<std::shared_ptr<Form> >  argValues;
			for (int i = 1; i < (int)elements.size(); ++i) {
				argValues.push_back(elements[i]->evaluate(vars));
			} 
			return func->apply(argValues, vars);
		}
	}
	
	throw std::string("runtime error"); 
}

std::string List::quote() {
	std::stringstream ret;
	ret << "(";
	
	if (elements.size() != 0) ret << elements[0]->quote();
	
	for (int i = 1; i < (int)elements.size(); ++i) {
		ret << " " << elements[i]->quote();
	}
	ret << ")";
	return ret.str();
}

std::shared_ptr<Form> List::getElement(int idx) {
	if (idx >= (int)elements.size()) throw std::string("runtime error");
	return elements[idx];
}
	
std::shared_ptr<List> List::tail(int idx) {
	if (idx > (int)elements.size()) throw std::string("runtime error");
	return std::make_shared<List>(std::vector<std::shared_ptr<Form> > (elements.begin() + idx, elements.end()));
}
std::shared_ptr<Pair> List::asPair() {
	if (elements.size() == 0) throw std::string("runtime error");
	return std::make_shared<Pair>(elements[0], tail(1));
}

Function::Function(std::vector<std::string>& _argNames,
				   std::shared_ptr<Form> _body) {
	for (size_t i = 0; i < _argNames.size(); ++i)  {
		for (size_t j = i + 1; j < _argNames.size(); ++j)  {
			if (_argNames[i] == _argNames[j]) throw std::string("syntax error");
		}
	} 		

	argNames = _argNames;
	body = _body;
}
	  
  
std::shared_ptr<Form> Function::evaluate(VariableMap& vars) {
	vars.doNothing();
	
	//self-evaluating
	return std::static_pointer_cast<Form>(shared_from_this());
}

std::shared_ptr<Form> Function::apply(std::vector<std::shared_ptr<Form> >& argValues, VariableMap& vars) {
	
	if (argValues.size() != argNames.size()) throw std::string("runtime error");
	int argCount = argNames.size();
	
	for (int i = 0; i < argCount; ++i) {
		vars.pushValue(argNames[i], argValues[i]);
	}
	
	for (auto var : context) {
		vars.pushValue(var.first, var.second);
	} 
	 
	std::shared_ptr<Form> ret = body->evaluate(vars); 
	
	for (auto var : context) { 
		context[var.first] = vars.getValue(var.first);
		vars.popValue(var.first);
	} 
	
	for (int i = 0; i < argCount; ++i) {
		argValues[i] = vars.getValue(argNames[i]);
		vars.popValue(argNames[i]);
	}
	return ret;
}
std::string Function::quote() {
	std::stringstream ret;
	int argCount = argNames.size();
	ret << "#<CLOSURE <anon> (";
	for(int i = 0; i < argCount; ++i) {
		ret << argNames[i];
		if (i < (int) argValues.size()) ret << "=" << argValues[i]->quote();
		if (i != argCount-1) ret << " ";
	}
	ret << ")" << body->quote() << ">";
	
	/*
	if (!context.empty()){
		ret <<"\nCONTEXT:";
		for(auto it: context)
		ret << it.first <<"="<<it.second->quote()<<",";
	}*/
	
	return ret.str();
}

Symbol::Symbol (std::string _name) {
	name = _name;
}
	
Symbol::~Symbol(){}
	
std::shared_ptr<Form> Symbol::evaluate(VariableMap& vars) {
	return vars.getValue(name);
}

FormType Symbol::getType(){
	return FormType::SymbolType;
} 

std::string Symbol::quote() {
	return name;
}		 

bool Symbol::asBoolean() {return true;}
	
Pair::Pair(std::shared_ptr<Form> _first, std::shared_ptr<Form> _second) {
	first = _first;
	second = _second;
}

std::shared_ptr<Form> Pair::evaluate(VariableMap&) {
	throw std::string("runtime error");
	return std::make_shared<List>();
}
	
std::string Pair::quote() {
	std::stringstream ans;
	ans << "(" << first->quote();
	std::shared_ptr<Form> right = second;
	
	while (right->getType() == FormType::PairType) {
		std::shared_ptr<Pair> rightPair = std::static_pointer_cast<Pair>(right);
		ans << " " << rightPair->car()->quote();
		right = rightPair->cdr();
	}
	
	std::string rightQuote = right->quote();
	if (!(rightQuote == "()")) {
		ans << " . " + rightQuote;
	}
	ans << ")";
	return ans.str(); 
}

bool Pair::isList() {
	if (second->getType() == FormType::ListType) return true;
	if (second->getType() == FormType::PairType) {
		return std::static_pointer_cast<Pair>(second)->isList();
	}
	return false;
}

std::shared_ptr<List> Pair::asList() {
	std::shared_ptr<List> list;
	if (second->getType() == FormType::ListType) {
		list = std::static_pointer_cast<List>(second);
	} else if (second->getType() == FormType::PairType) {
		list = std::static_pointer_cast<Pair>(second)->asList();
	} else { 
		throw std::string("runtime error");
	}
	
	std::vector<std::shared_ptr<Form> > elements = list->getElements();
	elements.insert(elements.begin(), first);
	return std::make_shared<List>(elements);
}

bool Expression::compare(std::shared_ptr<Form> left, std::shared_ptr<Form> right) {
	if (left->getType() == FormType::IntegerType && right->getType() == FormType::IntegerType) {
		int64_t leftValue = std::static_pointer_cast<Integer>(left)->getValue();
		int64_t rightValue = std::static_pointer_cast<Integer>(right)->getValue();
		
		if (name == "=") return (leftValue == rightValue);
		if (name == ">") return (leftValue > rightValue);
		if (name == "<") return (leftValue < rightValue);
		if (name == ">=") return (leftValue >= rightValue);
		if (name == "<=") return (leftValue <= rightValue);
		
		//return std::make_shared<Error>("runtime","");
	} else {
		throw std::string("runtime error");
	}	
	return false;
}

void Expression::getAllSymbols(std::shared_ptr<Form> form, std::set<std::string>& symbols){
	if (form->getType() == FormType::SymbolType) {
		symbols.insert(form->quote());
	} else if (form->getType() == FormType::ListType) {
		std::shared_ptr<List> list = std::static_pointer_cast<List>(form);
		std::vector<std::shared_ptr<Form> > elements = list->getElements();
		for (auto element : elements) {
			getAllSymbols(element, symbols);
		}
	} else if (form->getType() == FormType::ExpressionType) {
		std::shared_ptr<Expression> expr = std::static_pointer_cast<Expression>(form);
		std::vector<std::shared_ptr<Form> > arguments = expr->getArguments();
		for (auto argument : arguments) {
			getAllSymbols(argument, symbols);
		}
	} else if (form->getType() == FormType::FunctionType) {
		std::shared_ptr<Function> func = std::static_pointer_cast<Function>(form);
		getAllSymbols(func->getBody(), symbols);
	}
}

int64_t Expression::accumulate(std::shared_ptr<Form> arg, int64_t sum) {
	if (arg->getType() != FormType::IntegerType) throw std::string("runtime error");
	int64_t value = std::static_pointer_cast<Integer>(arg)->getValue();
	if (name == "+") {
		return sum + value;
	} else if (name == "-") {
		return sum - value;
	} else if (name == "*") {
		return sum * value;
	} else if (name == "/") {
		if (value == 0) throw std::string("runtime error");
		return sum / value;
	} else if (name == "min") {
		return ((value < sum) ? value : sum);
	} else if (name == "max") {
		return ((value > sum) ? value : sum);
	}
	
	throw std::string("runtime error");
}

std::vector<std::shared_ptr<Form> > Expression::evaluateAllArguments(VariableMap& vars) {
	std::vector<std::shared_ptr<Form> > ret;
	for (int i = 0; i < (int)arguments.size(); ++i) {
		ret.push_back(arguments[i]->evaluate(vars));
	} 
	return std::move(ret);
}

Expression::Expression(std::string _name) {
	name = _name;
}

Expression::Expression(std::string _name, std::vector<std::shared_ptr<Form> > _arguments) {
	name = _name;
	arguments = _arguments;
}	
 

std::shared_ptr<Form> Expression::evaluate(VariableMap& vars) {
	int argCount = arguments.size();
	
	if (name == "quote") {
		return arguments[0];
	}
	
	if (name == "define" || name == "set!") {
		//LAMBDA SUGAR
		if (argCount >= 2 && arguments[0]->getType() == FormType::ListType) {
			std::vector<std::shared_ptr<Form> > argSymbols = 
				(std::static_pointer_cast<List>(arguments[0]))->getElements(); 
			if (argSymbols.size() == 0) throw std::string("syntax error");
			if (argSymbols[0]->getType() != FormType::SymbolType)	throw std::string("syntax error");
			std::string functionName = argSymbols[0]->quote();
			std::shared_ptr<List> argNamesList = std::make_shared<List>();
			for (int i = 1; i < (int)argSymbols.size(); ++i) {
				if (argSymbols[i]->getType() != FormType::SymbolType) throw std::string("syntax error");
				argNamesList->addElement(std::make_shared<Symbol>(argSymbols[i]->quote()));
			} 

			std::shared_ptr<Expression> lambda = std::make_shared<Expression>("lambda");

			lambda->addArgument(argNamesList);
;
			
			for (int i = 1; i < argCount; ++i) {
				lambda->addArgument(arguments[i]);
			}

			
			vars.pushValue(functionName, lambda->evaluate(vars));
					
			return std::make_shared<List>();
		}
		
		if (argCount != 2) throw std::string("syntax error");
		
		if (arguments[0]->getType() != FormType::SymbolType) throw std::string("syntax error");
		std::string varName = (std::static_pointer_cast<Symbol>(arguments[0]))->quote();
		std::shared_ptr<Form> value = arguments[1]->evaluate(vars);
		
		if (name == "define") vars.pushValue(varName, value);
		if (name == "set!") vars.setValue(varName, value);
;
		return std::make_shared<List>();
	}
		
	
	if (name == "number?" || name == "boolean?" || name == "symbol?" || name == "list?") {
		if (argCount != 1) throw std::string("runtime error");
		
		FormType expectedType;
		if (name == "number?") expectedType = FormType::IntegerType;
		if (name == "boolean?") expectedType = FormType::BooleanType;
		if (name == "symbol?") expectedType = FormType::SymbolType;
		if (name == "list?") expectedType = FormType::ListType;
		
		return std::make_shared<Boolean>(arguments[0]->evaluate(vars)->getType() == expectedType);
	} else if (name == "pair?") {
		if (argCount != 1) throw std::string("runtime error");
		std::shared_ptr<Form> sample = arguments[0]->evaluate(vars);
		bool answer = false;
		if (sample->getType() == FormType::PairType) answer = true;
		if (sample->getType() == FormType::ListType) {
			if (sample->quote() != "()") answer= true;
		}
		
		return std::make_shared<Boolean>(answer);
	} else if (name == "null?") {
		if (argCount != 1) throw std::string("runtime error");
		std::string quote = arguments[0]->evaluate(vars)->quote();
		return std::make_shared<Boolean>(quote == "()");
	} else if (name == "+" || name == "-" || name == "*" || name == "/" || name == "min" || name == "max") {
		if (argCount == 0) {
			if (name == "+") return std::make_shared<Integer>(0);
			if (name == "*") return std::make_shared<Integer>(1);
			throw std::string("runtime error");			
		}
		
		auto evaledArgs = evaluateAllArguments(vars);
		if (evaledArgs[0]->getType() != FormType::IntegerType) throw std::string("runtime error");
		int64_t ans = std::static_pointer_cast<Integer>(evaledArgs[0])->getValue();
		
		 
		for (int i = 1; i < argCount; ++i) {
			ans = accumulate (evaledArgs[i], ans); 
		} 
		return std::make_shared<Integer>(ans); 
	} else if (name == "=" || name == ">=" || name == "<=" || name == "<" || name == ">") {
		auto evaledArgs = evaluateAllArguments(vars);
		bool ans = true;
		for (int i = 0; i < argCount - 1; ++i) {
			if (!compare(evaledArgs[i], evaledArgs[i+1])) {
				ans = false;
				break;
			}
		}
		return std::make_shared<Boolean>(ans);
	} else if (name == "abs") {
		if (argCount != 1) throw std::string("runtime error");
		std::shared_ptr<Form> arg = arguments[0]->evaluate(vars);
		if (arg->getType() != FormType::IntegerType) throw std::string("runtime error");
		int64_t intArg = std::static_pointer_cast<Integer>(arg)->getValue();
		if (intArg < 0) intArg = -intArg;
		return std::make_shared<Integer>(intArg);
	} else if (name == "not") {
		if (argCount != 1) throw std::string("runtime error");
		return std::make_shared<Boolean>( !(arguments[0]->evaluate(vars)->asBoolean()) );
	} else if (name == "and") { 
		if (argCount == 0) return std::make_shared<Boolean>(true);
		for (int i = 0; i < argCount; ++i) {
			std::shared_ptr<Form> value = arguments[i]->evaluate(vars);
			if (!(value->asBoolean()) || i == argCount - 1) return value;
		}
	} else if (name == "or") {
		if (argCount == 0) return std::make_shared<Boolean>(false);
		for (int i = 0; i < argCount; ++i) {
			std::shared_ptr<Form> value = arguments[i]->evaluate(vars);
			if (value->asBoolean() || i == argCount - 1) return value;
		}
	} else if (name == "if") {
		if (argCount < 2 || argCount > 3) throw std::string("syntax error");
		std::shared_ptr<Form> condition = arguments[0]->evaluate(vars);
		if (condition->asBoolean()) {
			return arguments[1]->evaluate(vars);
		}
		else {
			if (argCount == 2) return std::make_shared<List>();
			return arguments[2]->evaluate(vars);
		}
	} else if (name == "lambda") {
		if (argCount < 2) throw std::string("syntax error");
		if (arguments[0]->getType() != FormType::ListType) throw std::string("runtime error");
 
		std::vector<std::string> argNames;
		std::shared_ptr<List> argList = std::static_pointer_cast<List>(arguments[0]);  
		for (auto symbol : argList->getElements()) {
			if (symbol->getType() != FormType::SymbolType) throw std::string("runtime error");
			argNames.push_back(symbol->quote());
		}
		
		std::shared_ptr<Form> body;		
		if (argCount == 2) {
			body = arguments[1];
		} else {
			//explicit begin
			std::vector<std::shared_ptr<Form> > beginArgs;
			for (int i = 1; i < argCount; ++i) {
				beginArgs.push_back(arguments[i]);
			}
			body = std::static_pointer_cast<Form>(std::make_shared<Expression>("begin", beginArgs));
		} 
		
		std::shared_ptr<Function> function = std::make_shared<Function>(argNames, body);
		
		//store context
		std::map<std::string, std::shared_ptr<Form> > context;
		std::set<std::string> argNamesSet(argNames.begin(), argNames.end());
		std::set<std::string> symbolsInBody;
		getAllSymbols(body, symbolsInBody);
		for (auto symb : symbolsInBody) {
			if (vars.isDefined(symb) && argNamesSet.find(symb) == argNamesSet.end()) {
				context[symb] = vars.getValue(symb);
			}
		}
		if(!context.empty()) {
			function->setContext(context);
		}
		
		return function;
	} else if (name == "begin") {
		//evaluates all expressions, returns result of last
		if (argCount == 0) throw std::string("runtime error");
		for (int i = 0; i < argCount; i++) {
			std::shared_ptr<Form> res = arguments[i]->evaluate(vars);
			if (i == argCount - 1) return res;
		}
	} else if (name == "list") {
		return std::make_shared<List>(evaluateAllArguments(vars));
	} else if (name == "list-ref" || name == "list-tail") {
		if (argCount != 2) throw std::string("runtime error");
		auto evaledArgs = evaluateAllArguments(vars);
		
		if (evaledArgs[0]->getType() != FormType::ListType) throw std::string("runtime error");
		std::shared_ptr<List> list = std::static_pointer_cast<List>(evaledArgs[0]);
		if (evaledArgs[1]->getType() != FormType::IntegerType) throw std::string("runtime error");
		int idx = std::static_pointer_cast<Integer>(evaledArgs[1])->getValue();
		
		if (name == "list-ref") return list->getElement(idx);
		if (name == "list-tail") return list->tail(idx); 
	} else if (name == "cons") {
		if (argCount != 2) throw std::string("runtime error");
		auto evaledArgs = evaluateAllArguments(vars);
		return std::make_shared<Pair>(evaledArgs[0], evaledArgs[1]);
	} else if (name == "car" || name == "cdr") {
		if (argCount != 1) throw std::string("runtime error");
		std::shared_ptr<Form> arg = arguments[0]->evaluate(vars);
		if (arg->getType() != FormType::PairType) throw std::string("runtime error");
		std::shared_ptr<Pair> pair = std::static_pointer_cast<Pair>(arg);
		if (name == "car") return pair->car();
		if (name == "cdr") return pair->cdr();
	} else if (name == "set-car!" || name == "set-cdr!") {
		if (argCount != 2) throw std::string("runtime error");
		auto evaledArgs = evaluateAllArguments(vars);
		if (evaledArgs[0]->getType() != FormType::PairType) throw std::string("runtime error");
		std::shared_ptr<Pair> target = std::static_pointer_cast<Pair>(evaledArgs[0]);
		
		if (name == "set-car!") target->setCar(evaledArgs[1]);
		if (name == "set-cdr!") target->setCdr(evaledArgs[1]);
	} else {
		throw std::string("runtime error");
	} 
	
	return std::make_shared<List>();
}

std::string Expression::quote() {
	std::stringstream ret;
	ret << "(" << name;
	
	for (auto arg : arguments) {
		ret << " " << arg->quote();
	}
	ret << ")";
	return ret.str();
} 

