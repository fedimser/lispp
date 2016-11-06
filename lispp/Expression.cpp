#include "Expression.h"

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

