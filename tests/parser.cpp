#include <catch2/catch_test_macros.hpp>

#include <ascript/ast.hpp>
#include <ascript/parser.hpp>

#include <iostream>
#include <sstream>

TEST_CASE("Parse binary expressions") {
    {
        asc::Parser parser("1 + 2 * 3");
        auto program = parser.parse();

        CHECK(program->execute() == 7);
    }
    {
        asc::Parser parser("2 * 3 + 4");
        auto program = parser.parse();

        CHECK(program->execute() == 10);
    }

    {
        asc::Parser parser("1 + 2 * 3 + 4");
        auto program = parser.parse();

        CHECK(program->execute() == 11);
    }
}

TEST_CASE("Parse parenthesis") {
    {
        asc::Parser parser("(1 + 2) * 3");
        auto program = parser.parse();

        CHECK(program->execute() == 9);
    }
    {
        asc::Parser parser("(2 * 3) + 4");
        auto program = parser.parse();

        CHECK(program->execute() == 10);
    }

    {
        asc::Parser parser("7 * (1 + 2 * 2 + 3) + 5");
        auto program = parser.parse();

        CHECK(program->execute() == 61);
    }
}
