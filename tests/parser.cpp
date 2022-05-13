#include <catch2/catch_test_macros.hpp>

#include <ascript/ast.hpp>
#include <ascript/interpreter.hpp>
#include <ascript/parser.hpp>

TEST_CASE("Parse value") {
    CHECK(asc::from_str("24", 16) == asc::Value(36));
    CHECK(asc::from_str("8468", 10) == asc::Value(8468));

    CHECK(asc::from_str("1.25", 10) == asc::Value(1.25));

    CHECK_THROWS(asc::from_str("1.25", 16));
    CHECK_THROWS(asc::from_str("1.25.55", 10));
    CHECK_THROWS(asc::from_str("a3", 10));
}

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
