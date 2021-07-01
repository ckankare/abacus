#pragma once

#include <fmt/format.h>
#include <iostream>
#include <limits>

#include <array>

namespace aba {

class BigInt;

class BigUInt {
public:
    static constexpr std::array<char, 22> representation = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A',
                                                            'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K'};

    using data_t = uint32_t;
    static constexpr std::size_t data_size = 4;
    static constexpr std::size_t n_data_bits = std::numeric_limits<data_t>::digits;
    static constexpr std::size_t n_bits = data_size * n_data_bits;

    constexpr BigUInt() {}

    constexpr BigUInt(uint64_t value) {
        m_data[0] = static_cast<uint32_t>(value);
        m_data[1] = static_cast<uint32_t>(value >> 32);

        m_data[2] = 0;
        m_data[3] = 0;
    }

    constexpr BigUInt(const std::array<data_t, data_size>& data) : m_data(data) {}

    constexpr explicit BigUInt(const BigInt& value);

    friend constexpr std::strong_ordering operator<=>(const BigUInt& lhs, const BigUInt& rhs) {
        for (std::size_t i = data_size; i > 0; --i) {
            if (lhs.m_data[i - 1] > rhs.m_data[i - 1]) {
                return std::strong_ordering::greater;
            } else if (lhs.m_data[i - 1] < rhs.m_data[i - 1]) {
                return std::strong_ordering::less;
            }
        }

        return std::strong_ordering::equal;
    }

    friend constexpr bool operator==(const BigUInt& lhs, const BigUInt& rhs) {
        for (std::size_t i = 0; i < data_size; ++i) {
            if (lhs.m_data[i] != rhs.m_data[i]) {
                return false;
            }
        }
        return true;
    }

    friend constexpr bool operator!=(const BigUInt& lhs, const BigUInt& rhs) { return !(lhs == rhs); }

    friend constexpr BigUInt operator+(const BigUInt& lhs, const BigUInt& rhs) {
        BigUInt result(0);
        bool carry = false;
        for (std::size_t i = 0; i < data_size; ++i) {
            const uint32_t sum = lhs.m_data[i] + rhs.m_data[i];
            result.m_data[i] = sum + (carry ? 1 : 0);
            if (sum < lhs.m_data[i] || sum < rhs.m_data[i] || result.m_data[i] < sum) {
                carry = true;
            } else {
                carry = false;
            }
        }

        return result;
    }

    friend constexpr BigUInt operator-(const BigUInt& lhs, const BigUInt& rhs) { return lhs + (-rhs); }

    friend constexpr BigUInt operator*(const BigUInt& lhs, const BigUInt& rhs) {
        BigUInt result(0);

        // TODO overflow (wrap around?)
        for (std::size_t i = 0; i < data_size; ++i) {
            uint64_t intermediate = 0;

            for (std::size_t j = 0; j < data_size; ++j) {
                if ((i + j) >= data_size) {
                    continue;
                }
                intermediate += static_cast<uint64_t>(lhs.m_data[i]) * static_cast<uint64_t>(rhs.m_data[j]);
                result.m_data[i + j] += static_cast<uint32_t>(intermediate);
                intermediate = intermediate >> n_data_bits;
            }
        }
        return result;
    }

    friend constexpr BigUInt operator/(const BigUInt& lhs, const BigUInt& rhs) {
        return BigUInt::division(lhs, rhs).first;
    }

    friend constexpr BigUInt operator-(const BigUInt& lhs) { return ~lhs + BigUInt(1); }

    friend constexpr BigUInt operator%(const BigUInt& lhs, const BigUInt& rhs) {
        return BigUInt::division(lhs, rhs).second;
    }

    friend constexpr BigUInt operator~(const BigUInt& value) {
        BigUInt result;

        for (std::size_t i = 0; i < data_size; ++i) {
            result.m_data[i] = ~value.m_data[i];
        }

        return result;
    }

    friend constexpr BigUInt operator>>(const BigUInt& lhs, uint32_t rhs) {
        if (rhs == 0) {
            return lhs;
        }

        if (rhs == 32) {
            BigUInt result(0);

            for (std::size_t i = data_size - 1; i > 0; --i) {
                result.m_data[i - 1] = lhs.m_data[i];
            }
            return result;
        }

        if (rhs > 32) {
            return (lhs >> (rhs - 32) >> 32);
        }

        BigUInt result(0);

        uint32_t remainder = 0;
        for (std::size_t i = data_size; i > 0; --i) {
            result.m_data[i - 1] = (lhs.m_data[i - 1] >> rhs) | remainder;
            remainder = lhs.m_data[i - 1] << (32 - rhs);
        }
        return result;
    }

    friend constexpr BigUInt operator<<(const BigUInt& lhs, uint32_t rhs) {
        if (rhs == 0) {
            return lhs;
        }

        if (rhs == 32) {
            BigUInt result(0);

            for (std::size_t i = 0; i < data_size - 1; ++i) {
                result.m_data[i + 1] = lhs.m_data[i];
            }

            return result;
        }

        if (rhs > 32) {
            return (lhs << (rhs - 32) << 32);
        }

        BigUInt result(0);

        uint32_t remainder = 0;
        for (std::size_t i = 0; i < data_size; ++i) {
            result.m_data[i] = (lhs.m_data[i] << rhs) | remainder;
            remainder = lhs.m_data[i] >> (32 - rhs);
        }
        return result;
    }

    static constexpr std::pair<BigUInt, BigUInt> division(const BigUInt& lhs, const BigUInt& rhs) {
        BigUInt result(0);
        BigUInt rem = lhs;
        BigUInt dem = rhs;

        while (rem != BigUInt(0) && rem >= dem) {
            uint32_t shifted = 0;
            if (rem > dem) {
                while ((rem >> shifted) > dem) {
                    shifted += 1;
                }
                shifted -= 1;
            }
            result = result + (BigUInt(1) << shifted);
            rem = rem - (dem << shifted);
        }

        return {result, rem};
    }

    static constexpr BigUInt pow(const BigUInt& lhs, uint32_t exponent) {
        BigUInt result = 1;
        for (std::size_t i = 0; i < exponent; ++i) {
            result = result * lhs;
        }
        return result;
    }

    uint32_t digits(uint32_t base) const {
        if (*this == BigUInt(0)) {
            return 1;
        }

        auto constexpr_log2 = [](uint32_t value) {
            uint32_t result = 0;
            while (value != 0) {
                value = value >> 1;
                result += 1;
            }

            return result - 1;
        };

        BigUInt rem = *this;
        uint32_t digits = 1;
        const auto nb_bits = constexpr_log2((n_bits - 1) / constexpr_log2(base));
        uint32_t count = 1 << nb_bits;

        do {
            BigUInt base_exp = pow(BigUInt(base), count);
            if (rem >= base_exp) {
                digits += count;
                rem = rem / base_exp;
            }
            count = count >> 1;
        } while (count > 0);

        return digits;
    }

    std::string to_string(uint16_t base) const {
        std::string result;
        result.reserve(45);

        BigUInt value = *this;

        const BigUInt big_int_base(base);

        do {
            BigUInt remainder;
            std::tie(value, remainder) = BigUInt::division(value, big_int_base);
            result = representation[remainder.m_data[0]] + result;
        } while (value != BigUInt(0));

        return result;
    }

private:
    friend class BigInt;
    std::array<data_t, data_size> m_data;
};

class BigInt {
public:
    using data_t = uint32_t;
    static constexpr std::size_t data_size = 4;
    static constexpr std::size_t n_data_bits = std::numeric_limits<data_t>::digits;
    static constexpr std::size_t n_bits = data_size * n_data_bits;

    static constexpr BigInt min() {
        BigInt result(0);

        result.m_data[3] = 1 << 31;

        return result;
    }

    static constexpr BigInt max() {
        BigInt result(0);

        const auto last = ~static_cast<uint32_t>(0);

        for (auto& data : result.m_data) {
            data = last;
        }

        result.m_data[3] = result.m_data[3] >> 1;

        return result;
    }

    constexpr BigInt() {}

    constexpr BigInt(const std::array<data_t, data_size>& data) : m_data(data) {}

    constexpr BigInt(int64_t value) {
        m_data[0] = static_cast<uint32_t>(value);
        m_data[1] = static_cast<uint32_t>(value >> 32);
        if (value >= 0) {
            m_data[2] = 0;
            m_data[3] = 0;
        } else {
            m_data[2] = ~static_cast<uint32_t>(0);
            m_data[3] = ~static_cast<uint32_t>(0);
        }
    }

    constexpr explicit BigInt(const BigUInt& value) : m_data(value.m_data) {}

    friend constexpr std::strong_ordering operator<=>(const BigInt& lhs, const BigInt& rhs) {
        if (!lhs.is_negative() && rhs.is_negative()) {
            return std::strong_ordering::greater;
        } else if (lhs.is_negative() && !rhs.is_negative()) {
            return std::strong_ordering::less;
        }

        return (BigUInt(lhs) <=> BigUInt(rhs));
    }

    friend constexpr bool operator==(const BigInt& lhs, const BigInt& rhs) {
        for (std::size_t i = 0; i < data_size; ++i) {
            if (lhs.m_data[i] != rhs.m_data[i]) {
                return false;
            }
        }
        return true;
    }

    friend constexpr bool operator!=(const BigInt& lhs, const BigInt& rhs) { return !(lhs == rhs); }

    friend constexpr BigInt operator+(const BigInt& lhs, const BigInt& rhs) {
        return BigInt(BigUInt(lhs) + BigUInt(rhs));
    }

    friend constexpr BigInt operator-(const BigInt& lhs, const BigInt& rhs) { return lhs + (-rhs); }

    friend constexpr BigInt operator*(const BigInt& lhs, const BigInt& rhs) {
        BigUInt result;
        if (lhs.is_negative() && rhs.is_negative()) {
            result = (BigUInt(-lhs) * BigUInt(-rhs));
        } else if (lhs.is_negative()) {
            result = -(BigUInt(-lhs) * BigUInt(rhs));
        } else if (rhs.is_negative()) {
            result = -(BigUInt(lhs) * BigUInt(-rhs));
        } else {
            result = BigUInt(lhs) * BigUInt(rhs);
        }

        return BigInt(result);
    }

    friend constexpr BigInt operator/(const BigInt& lhs, const BigInt& rhs) { return BigInt::division(lhs, rhs).first; }

    friend constexpr BigInt operator-(const BigInt& lhs) { return ~lhs + BigInt(1); }

    friend constexpr BigInt operator%(const BigInt& lhs, const BigInt& rhs) {
        return BigInt::division(lhs, rhs).second;
    }

    friend constexpr BigInt operator~(const BigInt& value) {
        BigInt result;

        for (std::size_t i = 0; i < data_size; ++i) {
            result.m_data[i] = ~value.m_data[i];
        }

        return result;
    }

    friend constexpr BigInt operator>>(const BigInt& lhs, uint32_t rhs) {
        if (rhs == 0) {
            return lhs;
        }

        // NOTE We only have logical shift.

        return BigInt(BigUInt(lhs) >> rhs);
    }

    friend constexpr BigInt operator<<(const BigInt& lhs, uint32_t rhs) {
        if (rhs == 0) {
            return lhs;
        }

        return BigInt(BigUInt(lhs) << rhs);
    }

    static constexpr std::pair<BigInt, BigInt> division(const BigInt& lhs, const BigInt& rhs) {
        if (lhs.is_negative() && rhs.is_negative()) {
            auto [result, rem] = BigUInt::division(BigUInt(-lhs), BigUInt(-rhs));
            return {BigInt(result), -BigInt(rem)};
        }

        if (lhs.is_negative()) {
            auto [result, rem] = BigUInt::division(BigUInt(-lhs), BigUInt(rhs));
            ;
            return {-BigInt(result), -BigInt(rem)};
        }

        if (rhs.is_negative()) {
            auto [result, rem] = BigUInt::division(BigUInt(lhs), BigUInt(-rhs));
            return {-BigInt(result), BigInt(rem)};
        }

        auto [result, rem] = BigUInt::division(BigUInt(lhs), BigUInt(rhs));
        return {BigInt(result), BigInt(rem)};
    }

    static constexpr BigInt pow(const BigInt& lhs, uint32_t exponent) {
        return ((exponent % 2) == 0 ? BigInt(1) : BigInt(-1)) * BigInt(BigUInt::pow(BigUInt(lhs), exponent));
    }

    uint32_t digits(uint32_t base) const {
        if (is_negative()) {
            return BigUInt(-*this).digits(base);
        }
        return BigUInt(*this).digits(base);
    }

    std::string to_string(uint16_t base) const {
        BigUInt value;

        if (is_negative()) {
            value = BigUInt(-(*this));
        } else {
            value = BigUInt(*this);
        }

        return (is_negative() ? "-" : "") + value.to_string(base);
    }

    std::string to_string() const { return to_string(10); }

    static constexpr BigInt from_string(std::string_view str, uint32_t base = 10) {
        if (str.empty()) {
            return BigInt(0);
        }

        std::size_t pos = 0;
        bool negative = false;
        if (str[pos] == '-') {
            pos += 1;
            negative = true;
        }

        // TODO Use BigUInt
        BigInt result(0);

        auto char_to_number = [](char c) -> uint8_t {
            // TODO Support bases > 10
            return c - '0';
        };

        while (pos < str.size()) {
            result = result * base;
            uint32_t value = char_to_number(str[pos]);
            result = result + value;
            pos += 1;
        }

        if (negative) {
            result = -result;
        }

        return result;
    }

    long double to_double() const {
        long double result = 0;
        for (std::size_t i = 0; i < m_data.size(); ++i) {
            if (is_negative()) {
                result += static_cast<long double>(static_cast<uint32_t>(~m_data[i])) * std::pow(2, 32 * i);
            } else {
                result += static_cast<long double>(m_data[i]) * std::pow(2, 32 * i);
            }
        }

        if (is_negative()) {
            result = (result + 1) * -1.0;
        }

        return result;
    }

    void dump() const {
        for (std::size_t i = 0; i < m_data.size(); ++i) {
            std::cout << fmt::format("m_data[{}] = {:x} ({})", i, m_data[i], static_cast<int32_t>(m_data[i])) << '\n';
        }
    }

    constexpr bool is_negative() const {
        return m_data[3] > static_cast<uint32_t>(std::numeric_limits<int32_t>::max());
    }

private:
    friend class BigUInt;

    std::array<data_t, data_size> m_data;
};

// TODO add formatting

constexpr BigUInt::BigUInt(const BigInt& value) : m_data(value.m_data) {}

} // namespace aba
