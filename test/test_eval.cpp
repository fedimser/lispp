#include <gtest/gtest.h>

#include "lisp_test.h"

TEST_F(LispTest, Quote) {
    ExpectEq("(quote (1 2))", "(1 2)");
    
    //mine tests
    ExpectEq("(quote (1 2 3))", "(1 2 3)");   
}
