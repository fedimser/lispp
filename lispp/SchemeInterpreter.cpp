#include "SchemeInterpreter.h"

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
