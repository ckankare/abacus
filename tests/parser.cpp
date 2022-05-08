#include <catch2/catch_test_macros.hpp>

#include <ascript/ast.hpp>
#include <ascript/interpreter.hpp>
#include <ascript/parser.hpp>

TEST_CASE("Parse binary expressions") {
    asc::Interpreter interpreter;
    {
        asc::Parser parser("1 + 2 * 3");
        auto program = parser.parse();

        CHECK(program->execute(interpreter) == asc::Value(7));
    }
    {
        asc::Parser parser("2 * 3 + 4");
        auto program = parser.parse();

        CHECK(program->execute(interpreter) == asc::Value(10));
    }

    {
        asc::Parser parser("1 + 2 * 3 + 4");
        auto program = parser.parse();

        CHECK(program->execute(interpreter) == asc::Value(11));
    }
}

TEST_CASE("Parse parenthesis") {
    asc::Interpreter interpreter;
    {
        asc::Parser parser("(1 + 2) * 3");
        auto program = parser.parse();

        CHECK(program->execute(interpreter) == asc::Value(9));
    }
    {
        asc::Parser parser("(2 * 3) + 4");
        auto program = parser.parse();

        CHECK(program->execute(interpreter) == asc::Value(10));
    }

    {
        asc::Parser parser("7 * (1 + 2 * 2 + 3) + 5");
        auto program = parser.parse();

        CHECK(program->execute(interpreter) == asc::Value(61));
    }
}

TEST_CASE("Parse function declaration") {
    {
        asc::Parser parser("fn zero() { 0 }");
        auto program = parser.parse();
    }

    {
        asc::Parser parser("fn sum(a, b) { a + b }");
        auto program = parser.parse();
    }
}
