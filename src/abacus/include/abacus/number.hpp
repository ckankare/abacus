#pragma once

#include "big_int.hpp"

namespace aba {

class Number {
public:
    constexpr explicit Number(int64_t value) : m_mantissa(value), m_exponent(0){};

    constexpr Number(int64_t mantissa, int32_t exponent) : m_mantissa(mantissa), m_exponent(exponent) {}
    constexpr Number(BigInt mantissa, int32_t exponent) : m_mantissa(mantissa), m_exponent(exponent) {}

    friend constexpr std::strong_ordering operator<=>(const Number& lhs, const Number& rhs) {
        const auto value = lhs - rhs;

        // TODO Maybe normalize or something.

        if (value.m_mantissa > 0) {
            return std::strong_ordering::greater;
        } else if (value.m_mantissa < 0) {
            return std::strong_ordering::less;
        }

        return std::strong_ordering::equal;
    }

    friend constexpr bool operator==(const Number& lhs, const Number& rhs) {
        return lhs <=> rhs == std::strong_ordering::equal;
    }

    friend constexpr bool operator!=(const Number& lhs, const Number& rhs) { return !(lhs == rhs); }

    constexpr BigInt mantissa() const { return m_mantissa; }

    constexpr BigInt exponent() const { return BigInt(m_exponent); }

    constexpr long double to_double() const {
        return m_mantissa.to_double() * aba::pow<long double>(2.0, static_cast<long double>(m_exponent));
    }

    friend constexpr Number operator+(const Number& lhs, const Number& rhs) {
        if (lhs.m_exponent == rhs.m_exponent) {
            return Number(lhs.m_mantissa + rhs.m_mantissa, lhs.m_exponent);
        }

        // TODO Check for overflow.
        if (lhs.m_exponent > rhs.m_exponent) {
            return Number(lhs.m_mantissa * aba::pow(2, lhs.m_exponent - rhs.m_exponent) + rhs.m_mantissa,
                          rhs.m_exponent);
        } else {
            return Number(rhs.m_mantissa * aba::pow(2, rhs.m_exponent - lhs.m_exponent) + lhs.m_mantissa,
                          lhs.m_exponent);
        }
    }

    friend constexpr Number operator-(const Number& lhs, const Number& rhs) {
        if (lhs.m_exponent == rhs.m_exponent) {
            return Number(lhs.m_mantissa - rhs.m_mantissa, lhs.m_exponent);
        }

        // TODO Check for overflow.
        if (lhs.m_exponent > rhs.m_exponent) {
            return Number(lhs.m_mantissa * aba::pow(2, lhs.m_exponent - rhs.m_exponent) - rhs.m_mantissa,
                          rhs.m_exponent);
        } else {
            return Number(-rhs.m_mantissa * aba::pow(2, rhs.m_exponent - lhs.m_exponent) + lhs.m_mantissa,
                          lhs.m_exponent);
        }
    }

    friend constexpr Number operator*(const Number& lhs, const Number& rhs) {
        if (rhs.m_mantissa > 0 && lhs.m_mantissa > std::numeric_limits<int64_t>::max() / rhs.m_mantissa) {
            return Number(((lhs.m_mantissa + (2 << 16)) >> 16) * ((rhs.m_mantissa + (2 << 16)) >> 16),
                          lhs.m_exponent + rhs.m_exponent + 32);
        }
        return Number(lhs.m_mantissa * rhs.m_mantissa, lhs.m_exponent + rhs.m_exponent);
    }

    friend constexpr Number operator/(const Number& lhs, const Number& rhs) {
        return Number(lhs.m_mantissa / rhs.m_mantissa, lhs.m_exponent - rhs.m_exponent);
    }

    // inline std::string to_string(uint16_t base) const {
    //     // TODO Allow formatting.
    //     if (m_mantissa == 0) {
    //         return "0";
    //     }

    //     if (m_exponent == 0) {
    //         return m_mantissa.to_string(base);
    //     }

    //     std::string result;
    //     if (m_mantissa.is_negative()) {
    //         result.append(1, '-');
    //     }

    //     if (m_exponent > 0) {
    //         //
    //     } else {
    //         //
    //         result.append("0.");
    //     }

    //     return result;
    // }

    // inline std::string to_string() const { return to_string(10); }

private:
    BigInt m_mantissa{0};
    int32_t m_exponent;
};
} // namespace aba
