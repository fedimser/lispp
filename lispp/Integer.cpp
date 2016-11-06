#include "Integer.h"

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
