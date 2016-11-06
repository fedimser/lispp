project(lispp)

cmake_minimum_required(VERSION 2.8)

add_library(gtest
    test/gmock/gmock-gtest-all.cc
    test/gmock/gmock_main.cc)

# -Wall -Wextra -Werror - enable all compiler warnings and turn them into errors
# -fsanitize=undefined -fsanitize=address - detect UB and memory errors in runtime
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -Werror -g -fsanitize=undefined -fsanitize=address")

include_directories(.)
include_directories(test/gmock)

add_library(lispp
    lispp/Boolean.cpp
    lispp/Expression.cpp
    lispp/Form.cpp
    lispp/FormType.cpp
    lispp/Function.cpp
    lispp/Integer.cpp
    lispp/List.cpp
    lispp/SchemeInterpreter.cpp
    lispp/Symbol.cpp
    lispp/VariableMap.cpp
    lispp/Pair.cpp
)

add_executable(lisp_repl main.cpp)

target_link_libraries(lisp_repl lispp)

add_executable(run_tests
    test/lisp_test.cpp
    test/test_eval.cpp
    test/test_integer.cpp
    test/test_boolean.cpp
    test/test_symbol.cpp
    test/test_list.cpp
    test/test_control_flow.cpp
    test/test_lambda.cpp
)

target_link_libraries(run_tests gtest pthread lispp)
