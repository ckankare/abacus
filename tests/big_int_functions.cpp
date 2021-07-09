#include <catch2/catch_test_macros.hpp>

#include <abacus/big_int_functions.hpp>

TEST_CASE("Sqrt") {
    REQUIRE(aba::sqrt(aba::BigInt(0)).to_string() == "0");
    REQUIRE(aba::sqrt(aba::BigInt(1)).to_string() == "1");

    REQUIRE(aba::sqrt(aba::BigInt(16)).to_string() == "4");
    REQUIRE(aba::sqrt(aba::BigInt::from_string("38407718418775")).to_string() == "6197396");
    REQUIRE(aba::sqrt(aba::BigInt::from_string("53387484038438408480843484")).to_string() == "7306673938149");

    REQUIRE(aba::sqrt(aba::BigInt::from_string("170141183460469231731687303715884105727")).to_string() ==
            "13043817825332782212");
}

TEST_CASE("Root") {
    REQUIRE(aba::root(aba::BigInt(0), 3).to_string() == "0");
    REQUIRE(aba::root(aba::BigInt(64), 3).to_string() == "4");

    REQUIRE(aba::root(aba::BigInt::from_string("170141183460469231731687303715884105727"), 3).to_string() ==
            "5541191377756");

    REQUIRE(aba::root(aba::BigInt::from_string("170141183460469231731687303715884105727"), 11).to_string() == "2989");
}
