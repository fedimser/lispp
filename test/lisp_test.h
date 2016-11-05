#pragma once

#include <string>
#include <sstream>

#include <gtest/gtest.h>
#include "../SchemeInterpreter.cpp"

struct LispTest : public ::testing::Test {
    SchemeInterpreter inter;
    
    
    // initialize new interpreter here
    LispTest() {
		inter = SchemeInterpreter(); 
	}

    // evaluate expression and compare result to expected
    void ExpectEq(std::string expression, std::string result) {
        std::string got = inter.processForm(expression); 
        if (got != result) {
			FAIL() << "FAIL ExpectEq" << std::endl <<
			"Expression:" << expression << std::endl <<
			"Expected result: " << result << std::endl <<
			"Your result: " << got<< std::endl;
		}
    }

    // evaluate expression and check that there is not errors
    void ExpectNoError(std::string expression) {
        std::string got = inter.processForm(expression); 
        if (got == "syntax error") FAIL() <<"syntax error got but not expected\n";
        if (got == "runtime error") FAIL() <<"runtime error got but not expected\n";
        if (got == "name error") FAIL() <<"name error got but not expected\n";
    }

    // check that expression evaluation produces syntax error
    void ExpectSyntaxError(std::string expression) {
        ExpectEq(expression, "syntax error");
    }

    // check that expression evaluation produces runtime error
    void ExpectRuntimeError(std::string expression) {
        ExpectEq(expression, "runtime error");
    }

    // check that expression evaluation produces name error
    void ExpectUndefinedVariable(std::string expression) {
        ExpectEq(expression, "name error");
    }
};
