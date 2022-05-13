#pragma once

#include <array>
#include <memory>

namespace utils {

template <typename T, typename... Ts>
struct are_same : std::conjunction<std::is_same<T, Ts>...> {};

template <typename... Ts>
constexpr bool are_same_v = are_same<Ts...>::value;

template <typename T, typename... Args>
constexpr auto make_array(T&& arg, Args&&... args) {
    static_assert(are_same_v<T, Args...>, "All elements must be of same type");
    return std::array<std::decay_t<T>, sizeof...(args) + 1>{{std::forward<T>(arg), std::forward<Args>(args)...}};
}

template <int32_t I>
struct integer {
    static constexpr int32_t value = I;
    constexpr auto operator==(integer rhs) { return value == rhs.value; }
    constexpr auto operator!=(integer rhs) { return value != rhs.value; }
    constexpr auto operator<=>(integer rhs) { return value <=> rhs.value; }

    constexpr auto operator==(int32_t rhs) { return value == rhs; }
    constexpr auto operator!=(int32_t rhs) { return value != rhs; }
    constexpr auto operator<=>(int32_t rhs) { return value <=> rhs; }
};

template <int32_t I, int32_t J, typename F>
constexpr auto for_each(F f) {
    static_assert(J >= I);
    f(integer<I>{});
    if constexpr (I < J) {
        for_each<I + 1, J>(std::move(f));
    }
}

template <typename T1, typename T2>
constexpr T1 unique_cast(std::unique_ptr<T2>& ptr) {
    return dynamic_cast<T1>(ptr.get());
}

template <typename T1, typename T2>
constexpr const T1 unique_cast(const std::unique_ptr<T2>& ptr) {
    return dynamic_cast<T1>(ptr.get());
}
} // namespace utils
