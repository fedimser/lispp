#include "Pair.h" 
	
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
