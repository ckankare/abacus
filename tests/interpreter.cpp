#include <catch2/catch_test_macros.hpp>

#include <ascript/ast.hpp>
#include <ascript/interpreter.hpp>
#include <ascript/parser.hpp>

TEST_CASE("Simple identifier") {
    asc::Interpreter interpreter;
    interpreter.push();
    interpreter.define_variable("pi", 314);
    {
        asc::Parser parser("1 + pi");
        auto program = parser.parse();

        CHECK(program->execute(interpreter) == 315);
    }
}
