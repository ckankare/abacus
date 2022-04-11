#include <catch2/catch_test_macros.hpp>

#include <abacus/number.hpp>

TEST_CASE("Number Constructors") {
    REQUIRE(aba::Number(6841735714).to_double() == 6841735714.0);
    REQUIRE(aba::Number(3, 44).to_double() == 52776558133248.0);
    REQUIRE(aba::Number(3, -44).to_double() == 0.00000000000017053025658242404460906982421875);
}

TEST_CASE("Number Comparisons") {
    REQUIRE(aba::Number(0) == aba::Number(0));
    REQUIRE(aba::Number(10, 0) == aba::Number(10));
    REQUIRE(aba::Number(10, 0) <= aba::Number(100));

    REQUIRE(aba::Number(14, 0) == aba::Number(7, 1));
    REQUIRE(aba::Number(-14, 0) == aba::Number(-7, 1));

    REQUIRE(aba::Number(2456, 4124) == aba::Number(2456, 4124));
    REQUIRE(aba::Number(6841735714) > aba::Number(3, -44));
    REQUIRE(aba::Number(3, 44) > aba::Number(3, -44));
    REQUIRE(aba::Number(3, 44) == aba::Number(3, 44));

    REQUIRE(aba::Number(234523545235, 878483723) == aba::Number(234523545235, 878483723));
    REQUIRE(aba::Number(234523545235, 878483723) > aba::Number(234523545234, 878483723));
    REQUIRE(aba::Number(234523545235, 878483723) < aba::Number(234523545235, 878483724));

    REQUIRE(aba::Number(234523545235, -878483723) == aba::Number(234523545235, -878483723));
    REQUIRE(aba::Number(234523545235, -878483723) > aba::Number(234523545234, -878483723));
    REQUIRE(aba::Number(234523545235, -878483723) > aba::Number(234523545235, -878483724));

    REQUIRE(aba::Number(-234523545235, -878483723) == aba::Number(-234523545235, -878483723));
    REQUIRE(aba::Number(-234523545235, -878483723) < aba::Number(-234523545234, -878483723));
    REQUIRE(aba::Number(-234523545235, -878483723) < aba::Number(-234523545235, -878483724));
}

TEST_CASE("Number to_string") {
    // REQUIRE(aba::Number(6841735714).to_string() == "6841735714.0");
    // REQUIRE(aba::Number(3, 44).to_string() == "52776558133248.0");
    // REQUIRE(aba::Number(3, -44).to_string() == "0.00000000000017053025658242404460906982421875");
}

TEST_CASE("Number Addition") {
    //
}

TEST_CASE("Number Subtraction") {
    //
}

TEST_CASE("Number Multiplication") {
    //
}

TEST_CASE("Number Division") {
    REQUIRE(aba::Number(-346245245546, -43465712) / aba::Number(-346245245546, -43465712) == aba::Number(1, 0));
    REQUIRE(aba::Number(-346245245546, -43465710) / aba::Number(-346245245546, -43465712) == aba::Number(4, 0));
    REQUIRE(aba::Number(-346245245546, -43465712) / aba::Number(-346245245546, -43465710) == aba::Number(1, -2));
}
