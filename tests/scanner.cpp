#include <catch2/catch_test_macros.hpp>

#include <ascript/scanner.hpp>

TEST_CASE("Empty scanner") {
    asc::Scanner scanner("");
    REQUIRE(scanner.next().character == '\0');
}
