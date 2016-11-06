#include "List.h"

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
