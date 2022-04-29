#include <catch2/catch_test_macros.hpp>

#include <ascript/function.hpp>

static asc::Value double_sum(std::vector<asc::Value> args) {
    int64_t sum = 0;
    for (const auto& arg : args) {
        sum += 2 * arg.as_int64();
    }
    return asc::Value(sum);
}

TEST_CASE("Simple native function") {
    asc::NativeFunction func(double_sum);
    auto result = func.call({asc::Value(13), asc::Value(44)});
    REQUIRE(result == asc::Value(26 + 88));
}
