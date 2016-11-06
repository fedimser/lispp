#include "Boolean.h"

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
