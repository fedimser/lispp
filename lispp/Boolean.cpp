#include "Boolean.h"

std::shared_ptr<Form> Boolean::evaluate(VariableMap& vars) {
	vars.doNothing();
	return std::make_shared<Boolean>(value);
}

std::string Boolean::quote() {
	if (value) {
		return "#t";
	} else {
		return "#f";
	}
}
