#include <catch2/catch_test_macros.hpp>

#include <ascript/ast.hpp>
#include <ascript/interpreter.hpp>
#include <ascript/parser.hpp>

#include <iostream>
#include <sstream>

TEST_CASE("Simple identifier") {
    asc::Interpreter interpreter;
    interpreter.define_variable("pi", asc::Value(314));
    {
        asc::Parser parser("1 + pi");
        auto program = parser.parse();

        CHECK(program->execute(interpreter) == asc::Value(315));
    }
}

static asc::Value double_sum(std::vector<asc::Value> args) {
    int64_t sum = 0;
    for (const auto& arg : args) {
        sum += 2 * arg.as_int64();
    }
    return asc::Value(sum);
}

TEST_CASE("Call simple native function") {
    asc::Interpreter interpreter;
    interpreter.define_function("double_sum", std::make_unique<asc::NativeFunction>(double_sum));
    {
        asc::Parser parser("double_sum(41, 3)");
        auto program = parser.parse();

        CHECK(program->execute(interpreter) == asc::Value(41 * 2 + 3 * 2));
    }
}

TEST_CASE("Execute function declaration") {
    {
        asc::Interpreter interpreter;
        asc::Parser parser("fn two() { 2 }"
                           "3 + two() * 4");
        auto program = parser.parse();
        CHECK(program->execute(interpreter) == asc::Value(11));
    }

    {
        asc::Interpreter interpreter;
        asc::Parser parser("fn sum(a, b) { a + b }"
                           "3 * sum(7,15) + sum(2*2, 1*3)");
        auto program = parser.parse();
        CHECK(program->execute(interpreter) == asc::Value(66 + 7));
    }

    {
        asc::Interpreter interpreter;
        asc::Parser parser("fn sum(a, b) { a + b }"
                           "fn two() { 2 }"
                           "two() * sum(two(), sum(5,3)) - sum(two(), two())");
        auto program = parser.parse();
        CHECK(program->execute(interpreter) == asc::Value(16));
    }
}

TEST_CASE("Assignment") {
    {
        asc::Interpreter interpreter;
        asc::Parser parser("a = 5;"
                           "b = a + 2;"
                           "a * 3 + b");
        auto program = parser.parse();
        CHECK(program->execute(interpreter) == asc::Value(22));
    }

    {
        asc::Interpreter interpreter;
        asc::Parser parser("foo = 3;"
                           "foo = 2 * foo + 7;"
                           "{foo = foo + 1;}"
                           "foo");
        auto program = parser.parse();
        CHECK(program->execute(interpreter) == asc::Value(14));
    }
}
