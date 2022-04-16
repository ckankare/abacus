#include <catch2/catch_test_macros.hpp>

#include <ascript/ast.hpp>
#include <ascript/parser.hpp>

#include <iostream>
#include <sstream>

TEST_CASE("Parse simple expression") {
    {
        asc::Parser parser("1 + 2 * 3");
        auto program = parser.parse();

        std::stringstream builder;
        program->dump(0, builder);
        std::cout << builder.str();
    }
    {
        asc::Parser parser("1 * 2 + 3");
        auto program = parser.parse();

        std::stringstream builder;
        program->dump(0, builder);
        std::cout << builder.str();
    }

    {
        asc::Parser parser("1 + 2 * 3 + 4");
        auto program = parser.parse();

        std::stringstream builder;
        program->dump(0, builder);
        std::cout << builder.str();
    }
}
