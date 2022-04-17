#include <catch2/catch_test_macros.hpp>

#include <ascript/ast.hpp>
#include <ascript/interpreter.hpp>
#include <ascript/parser.hpp>

#include <iostream>
#include <sstream>

TEST_CASE("Parse binary expressions") {
    asc::Interpreter interpreter;
    {
        asc::Parser parser("1 + 2 * 3");
        auto program = parser.parse();

        CHECK(program->execute(interpreter) == 7);
    }
    {
        asc::Parser parser("2 * 3 + 4");
        auto program = parser.parse();

        CHECK(program->execute(interpreter) == 10);
    }

    {
        asc::Parser parser("1 + 2 * 3 + 4");
        auto program = parser.parse();

        CHECK(program->execute(interpreter) == 11);
    }
}

TEST_CASE("Parse parenthesis") {
    asc::Interpreter interpreter;
    {
        asc::Parser parser("(1 + 2) * 3");
        auto program = parser.parse();

        CHECK(program->execute(interpreter) == 9);
    }
    {
        asc::Parser parser("(2 * 3) + 4");
        auto program = parser.parse();

        CHECK(program->execute(interpreter) == 10);
    }

    {
        asc::Parser parser("7 * (1 + 2 * 2 + 3) + 5");
        auto program = parser.parse();

        CHECK(program->execute(interpreter) == 61);
    }
}
