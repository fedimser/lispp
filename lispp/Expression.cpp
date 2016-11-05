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

int64_t Expression::accumulateInitial() {
	if (name == "+") return 0;
	if (name == "*") return 1;
	if (name == "max" || name == "min") {
		if (arguments.size() == 0) throw std::string("runtime error");
		if (arguments[0]->getType() != FormType::IntegerType) throw std::string("runtime error");
		return std::static_pointer_cast<Integer>(arguments[0])->getValue();
	}
	
	throw std::string("runtime error");
}

int64_t Expression::accumulate(std::shared_ptr<Form> arg, int64_t sum) {
	if (arg->getType() != FormType::IntegerType) throw std::string("runtime error");
	int64_t value = std::static_pointer_cast<Integer>(arg)->getValue();
	if (name == "+") {
		return sum + value;
	} else if (name == "*") {
		return sum * value;
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
 

std::shared_ptr<Form> Expression::evaluate(VariableMap& vars) {
	int argCount = arguments.size();
	
	if (name == "quote") {
		return arguments[0];
	}
	
	if (name == "define" || name == "set!") {
		if (argCount != 2) throw std::string("syntax error");
		if (arguments[0]->getType() != FormType::SymbolType) throw std::string("syntax error");
		std::string varName = (std::static_pointer_cast<Symbol>(arguments[0]))->quote();
		std::shared_ptr<Form> value = arguments[1]->evaluate(vars);
		
		if (name == "define") vars.pushValue(varName, value);
		if (name == "set!") vars.setValue(varName, value);
		 
		return std::make_shared<List>();
	}
		
	
	if (name == "number?" || name == "boolean?" || name == "symbol?") {
		if (argCount != 1) throw std::string("runtime error");
		
		FormType expectedType = FormType::DefaultType;
		if (name == "number?") expectedType = FormType::IntegerType;
		if (name == "boolean?") expectedType = FormType::BooleanType;
		if (name == "symbol?") expectedType = FormType::SymbolType;
		
		return std::make_shared<Boolean>(arguments[0]->evaluate(vars)->getType() == expectedType);
	} else if (name == "+" || name == "*" || name == "min" || name == "max") {
		auto evaledArgs = evaluateAllArguments(vars);
		int64_t ans = accumulateInitial();
		for (int i = 0; i < argCount; ++i) {
			ans = accumulate (evaledArgs[i], ans);
		}
		return std::make_shared<Integer>(ans); 
		
	} else if (name == "-" || name == "/") {
		if (argCount != 2) throw std::string("runtime error");
		auto evaledArgs = evaluateAllArguments(vars);
		if (evaledArgs[0]->getType() != FormType::IntegerType) throw std::string("runtime error");
		if (evaledArgs[1]->getType() != FormType::IntegerType) throw std::string("runtime error");
		
		int64_t leftValue = std::static_pointer_cast<Integer>(evaledArgs[0])->getValue();
		int64_t rightValue = std::static_pointer_cast<Integer>(evaledArgs[1])->getValue();
		int64_t result = 0;	 
		if (name == "-") {
			result = leftValue - rightValue; 
		} else if (name == "/") {
			if (rightValue == 0) throw std::string("runtime error");
			result = leftValue / rightValue;
		}
		return std::make_shared<Integer>(result);
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
		if (arg < 0) intArg = -intArg;
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
		if (argCount !=2) throw std::string("runtime error");
		//if (args[0]->getType() != FormType::ListType) throw std::string("runtime error");
		//if (args[1]->getType() != FormType::ExpressionType) throw std::string("runtime error");
		
		std::vector<std::string> argNames;
		std::shared_ptr<List> argList = std::static_pointer_cast<List>(arguments[0]); 
		std::shared_ptr<Expression> body = std::static_pointer_cast<Expression>(arguments[1]); 
		for (auto symbol : argList->getElements()) {
			if (symbol->getType() != FormType::SymbolType) throw std::string("runtime error");
			argNames.push_back(symbol->quote());
		}
		return std::make_shared<Function>(argNames, body);
	}
	else {
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

