#include <string>

#include <catch2/catch_test_macros.hpp>

#include <abacus/big_int.hpp>

namespace {
const std::string min_str = "-170141183460469231731687303715884105728";
const std::string max_str = "170141183460469231731687303715884105727";
} // namespace

TEST_CASE("Division/Remainder function") {
    auto a = aba::BigInt(0);
    auto b = aba::BigInt(2);
    auto [result, rem] = aba::BigInt::division(a, b);
    REQUIRE(result == 0);
    REQUIRE(rem == 0);

    a = aba::BigInt(1);
    b = aba::BigInt(1);
    std::tie(result, rem) = aba::BigInt::division(a, b);
    REQUIRE(result == 1);
    REQUIRE(rem == 0);

    a = aba::BigInt(7);
    b = aba::BigInt(3);
    std::tie(result, rem) = aba::BigInt::division(a, b);
    REQUIRE((b * result + rem) == a);

    a = aba::BigInt(7);
    b = aba::BigInt(-3);
    std::tie(result, rem) = aba::BigInt::division(a, b);
    REQUIRE((b * result + rem) == a);

    std::tie(result, rem) = aba::BigInt::division(10, 10);
    REQUIRE(result == 1);
    REQUIRE(rem == 0);

    std::tie(result, rem) = aba::BigInt::division(0, -50);
    REQUIRE(result == 0);
    REQUIRE(rem == 0);

    std::tie(result, rem) = aba::BigInt::division(49, -50);
    REQUIRE(result == 0);
    REQUIRE(rem == 49);

    a = aba::BigInt(52464312314);
    b = aba::BigInt(54357784);
    std::tie(result, rem) = aba::BigInt::division(a, b);
    REQUIRE((b * result + rem) == a);

    a = aba::BigInt(-52464312314);
    b = aba::BigInt(54357784);
    std::tie(result, rem) = aba::BigInt::division(a, b);
    REQUIRE((b * result + rem) == a);

    a = aba::BigInt(52464312314);
    b = aba::BigInt(-54357784);
    std::tie(result, rem) = aba::BigInt::division(a, b);
    REQUIRE((b * result + rem) == a);

    a = aba::BigInt(-52464312314);
    b = aba::BigInt(-54357784);
    std::tie(result, rem) = aba::BigInt::division(a, b);
    REQUIRE((b * result + rem) == a);

    a = aba::BigInt(5284847464312314);
    b = aba::BigInt(-54784);
    std::tie(result, rem) = aba::BigInt::division(a, b);
    REQUIRE((b * result + rem) == a);

    a = aba::BigInt::min();
    b = aba::BigInt::min();
    std::tie(result, rem) = aba::BigInt::division(a, b);
    REQUIRE(result == 1);
    REQUIRE(rem == 0);
    REQUIRE((b * result + rem) == a);

    a = aba::BigInt::min();
    b = aba::BigInt(2);
    std::tie(result, rem) = aba::BigInt::division(a, b);
    REQUIRE((b * result + rem) == a);
}

TEST_CASE("Tests to_string") {
    REQUIRE(aba::BigInt(0).to_string() == "0");
    REQUIRE(aba::BigInt(-1).to_string() == "-1");
    REQUIRE(aba::BigInt(1).to_string() == "1");

    REQUIRE(aba::BigInt(-10).to_string() == "-10");
    REQUIRE(aba::BigInt(10).to_string() == "10");

    REQUIRE(aba::BigInt(-3818431).to_string() == "-3818431");
    REQUIRE(aba::BigInt(81324).to_string() == "81324");

    REQUIRE(aba::BigInt(2'147'483'647).to_string() == "2147483647");
    REQUIRE(aba::BigInt(-2'147'483'648).to_string() == "-2147483648");

    REQUIRE(aba::BigInt(2'147'483'648).to_string() == "2147483648");
    REQUIRE(aba::BigInt(-2'147'483'649).to_string() == "-2147483649");

    REQUIRE(aba::BigInt(8478146171723404).to_string() == "8478146171723404");
    REQUIRE(aba::BigInt(-8478146171723404).to_string() == "-8478146171723404");

    REQUIRE(aba::BigInt::min().to_string() == min_str);
    REQUIRE(aba::BigInt::max().to_string() == max_str);
    REQUIRE(aba::BigInt::max().to_string(16) == "7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF");
}

TEST_CASE("Digits") {
    REQUIRE(aba::BigInt::min().digits(10) == 39);
    REQUIRE(aba::BigInt::max().digits(10) == 39);

    REQUIRE(aba::BigInt::min().digits(16) == 32);
    REQUIRE(aba::BigInt::max().digits(16) == 32);

    for (uint32_t base = 2; base < 20; ++base) {
        REQUIRE(aba::BigInt(0).digits(base) == 1);
        for (uint32_t exp = 1; exp < 10; ++exp) {
            REQUIRE(aba::BigInt(1 * static_cast<uint64_t>(std::pow(base, exp))).digits(base) == exp + 1);
        }
    }
}

TEST_CASE("Addition") {
    aba::BigInt a(84981);
    aba::BigInt b(21);
    auto result = a + b;
    REQUIRE(result.to_double() == 85002.0);

    a = -1;
    b = -2;
    result = a + b;
    REQUIRE(result.to_double() == -3.0);

    a = 3'840'498'350;
    b = -2'353'417'158;
    result = a + b;
    REQUIRE(result.to_double() == 1'487'081'192.0);

    a = -3'840'498'350'123;
    b = 2'353'417'158'234;
    result = b + a;
    REQUIRE(result.to_double() == -1'487'081'191'889.0);

    a = 3'840'498'789'350;
    b = -2'353'417'158;
    result = a + b;
    REQUIRE(result.to_double() == 3'838'145'372'192.0);

    a = aba::BigInt::min(); // is -170141183460469231731687303715884105728
    b = aba::BigInt::max(); // is  170141183460469231731687303715884105727
    result = a + b;

    REQUIRE(result.to_double() == -1.0);
}

TEST_CASE("Subtraction") {
    aba::BigInt a(84981);
    aba::BigInt b(21);
    auto result = a - b;
    REQUIRE(result.to_double() == 84960.0);

    a = -1;
    b = -2;
    result = a - b;
    REQUIRE(result.to_double() == 1.0);

    a = 3'840'498'350'123;
    b = -2'353'417'158'345;
    result = a - b;
    REQUIRE(result.to_double() == 6'193'915'508'468.0);

    a = -3'840'498'350;
    b = 2'353'417'158;
    result = b - a;
    REQUIRE(result.to_double() == 6'193'915'508.0);

    a = 3'840'498'789'350;
    b = -2'353'417'158;
    result = a + b;
    REQUIRE(result.to_double() == 3'838'145'372'192.0);

    a = aba::BigInt::min(); // is -170141183460469231731687303715884105728
    b = aba::BigInt::max(); // is  170141183460469231731687303715884105727
    result = a - a;
    REQUIRE(result.to_double() == 0.0);

    result = b - b;
    REQUIRE(result.to_double() == 0.0);
}

TEST_CASE("Multiplication") {
    REQUIRE((aba::BigInt(0) * aba::BigInt(7)).to_string() == "0");
    REQUIRE((aba::BigInt(1) * aba::BigInt(313488544)).to_string() == "313488544");

    REQUIRE((aba::BigInt(50) * aba::BigInt(7)).to_double() == 350.0);
    REQUIRE((aba::BigInt(-50) * aba::BigInt(7)).to_double() == -350.0);
    REQUIRE((aba::BigInt(50) * aba::BigInt(-7)).to_double() == -350.0);
    REQUIRE((aba::BigInt(-50) * aba::BigInt(-7)).to_double() == 350.0);

    REQUIRE((aba::BigInt(9481411248764) * aba::BigInt(32)).to_double() == 303'405'159'960'448.0);
    REQUIRE((aba::BigInt(-9481411248764) * aba::BigInt(32)).to_double() == -303'405'159'960'448.0);
    REQUIRE((aba::BigInt(9481411248764) * aba::BigInt(-32)).to_double() == -303'405'159'960'448.0);
    REQUIRE((aba::BigInt(-9481411248764) * aba::BigInt(-32)).to_double() == 303'405'159'960'448.0);

    REQUIRE((aba::BigInt(89644081819840101) * aba::BigInt(68781054)).to_string() == "6165814432430840258246454");
    REQUIRE((aba::BigInt(-89644081819840101) * aba::BigInt(68781054)).to_string() == "-6165814432430840258246454");
    REQUIRE((aba::BigInt(89644081819840101) * aba::BigInt(-68781053)).to_string() == "-6165814342786758438406353");

    REQUIRE((aba::BigInt::min() * aba::BigInt(1)).to_string() == min_str);
    REQUIRE((aba::BigInt::max() * aba::BigInt(-1)).to_string() == "-" + max_str);
}

TEST_CASE("Division") {
    REQUIRE((aba::BigInt(50) / aba::BigInt(7)).to_double() == 7.0);
    REQUIRE((aba::BigInt(-50) / aba::BigInt(7)).to_double() == -7.0);
    REQUIRE((aba::BigInt(50) / aba::BigInt(-7)).to_double() == -7.0);
    REQUIRE((aba::BigInt(-50) / aba::BigInt(-7)).to_double() == 7.0);

    REQUIRE((aba::BigInt(3513040135108) / aba::BigInt(284083180)).to_double() == 12'366.0);
    REQUIRE((aba::BigInt(-3513040135108) / aba::BigInt(284083180)).to_double() == -12'366.0);
    REQUIRE((aba::BigInt(3513040135108) / aba::BigInt(-284083180)).to_double() == -12'366.0);
    REQUIRE((aba::BigInt(-3513040135108) / aba::BigInt(-284083180)).to_double() == 12'366.0);

    REQUIRE((aba::BigInt::min() / aba::BigInt(1)).to_string() == min_str);
    REQUIRE((aba::BigInt::max() / aba::BigInt(1)).to_string() == max_str);
    REQUIRE((aba::BigInt::max() / aba::BigInt(-1)).to_string() == "-" + max_str);

    REQUIRE((aba::BigInt::min() / aba::BigInt::from_string("-521974568129211984565698731231236587")).to_string() ==
            "325");
    REQUIRE((aba::BigInt::max() / aba::BigInt::from_string("-51224982346756092387132111123232221")).to_string() ==
            "-3321");
    REQUIRE((aba::BigInt::max() / aba::BigInt::from_string("51224982346756092387132111123232221")).to_string() ==
            "3321");
}

TEST_CASE("Comparisons") {
    REQUIRE(aba::BigInt(0) > aba::BigInt(-1));
    REQUIRE(aba::BigInt(-1) < aba::BigInt(0));
    REQUIRE(aba::BigInt(-1) < aba::BigInt(1));

    REQUIRE(aba::BigInt(0) == aba::BigInt(0));
    REQUIRE(aba::BigInt(-1) == aba::BigInt(-1));
    REQUIRE(aba::BigInt(1) == aba::BigInt(1));

    REQUIRE(aba::BigInt(-3'840'498'789'351) < aba::BigInt(-3'840'498'789'350));
    REQUIRE(aba::BigInt(-3'840'498'789'350) > aba::BigInt(-3'840'498'789'351));

    REQUIRE(aba::BigInt(3'840'789'498'350) == aba::BigInt(3'840'789'498'350));
    REQUIRE(aba::BigInt(3'789'840'498'351) > aba::BigInt(3'789'840'498'350));
    REQUIRE(aba::BigInt(3'789'840'498'350) < aba::BigInt(3'789'840'498'351));

    REQUIRE(aba::BigInt(42) < aba::BigInt(3'789'840'498'351));
    REQUIRE(aba::BigInt(3'789'840'498'351) > aba::BigInt(-3'789'840'498'350));
    REQUIRE(aba::BigInt(3'789'840'498'351) > aba::BigInt(-42));

    REQUIRE(aba::BigInt::from_string("53387484038438408480843484") ==
            aba::BigInt::from_string("53387484038438408480843484"));
    REQUIRE(aba::BigInt::from_string("53387484038438408480843484") !=
            aba::BigInt::from_string("53387484038438408480843485"));

    REQUIRE(aba::BigInt::min() < aba::BigInt::max());
    REQUIRE(aba::BigInt::min() < -aba::BigInt::max());
    REQUIRE(aba::BigInt::min() == -aba::BigInt::max() - aba::BigInt(1));

    REQUIRE(aba::BigInt::max() == aba::BigInt::max());
}

TEST_CASE("From string") {
    REQUIRE(aba::BigInt::from_string("-170141183460469231731687303715884105728") == aba::BigInt::min());
    REQUIRE(aba::BigInt::from_string("170141183460469231731687303715884105727") == aba::BigInt::max());
}
