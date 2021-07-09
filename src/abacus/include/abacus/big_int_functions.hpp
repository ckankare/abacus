#pragma once

#include "big_int.hpp"

namespace aba {

constexpr BigInt root(const BigInt& value, int32_t n) {
    if (value == 0) {
        return BigInt(0);
    }

    // Newton–Raphson method (find roots/solutions for f(x) = x^n - value = 0) by
    // calculating x_n+1 = x_n - f(x_n)/f'(x_n) until x_n+1 >= x_n.
    // f(x)/f'(x) = (x^n - value) / (n*x^(n-1)) = x/n - value/(n*x^(n-1))
    BigInt x_n = value / value.digits(n) + 1;
    while (true) {
        auto next = x_n - x_n / n + value / (n * aba::BigInt::pow(x_n, n - 1));
        if (next >= x_n) {
            break;
        }
        x_n = next;
    }

    return x_n;
};

constexpr BigInt sqrt(const BigInt& value) { return root(value, 2); };

} // namespace aba
