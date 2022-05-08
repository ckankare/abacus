#include <catch2/catch_test_macros.hpp>

#include <ascript/ast.hpp>
#include <ascript/interpreter.hpp>
#include <ascript/parser.hpp>

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
