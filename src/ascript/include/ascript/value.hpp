#pragma once

#include <cassert>
#include <cstdint>
#include <string>
#include <variant>

#include <fmt/format.h>
#include <fmt/ostream.h>

namespace asc {

template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};

class Value {
public:
    constexpr Value() {}
    explicit Value(std::string value) : m_data(std::move(value)) {}
    constexpr explicit Value(double value) : m_data(value) {}
    constexpr explicit Value(int32_t value) : Value(static_cast<int64_t>(value)) {}
    constexpr explicit Value(int64_t value) : m_data(value) {}

    Value operator+(const Value& rhs) {
        if (m_data.index() != rhs.m_data.index()) {
            return Value();
        }

        return std::visit([&](auto arg) { return Value(arg + std::get<decltype(arg)>(rhs.m_data)); }, m_data);
    }
    Value operator-(const Value& rhs) {
        if (m_data.index() != rhs.m_data.index()) {
            return Value();
        }

        return std::visit(overloaded{
                              [](const std::string&) { return Value(); },                                //
                              [&](auto arg) { return Value(arg - std::get<decltype(arg)>(rhs.m_data)); } //
                          },
                          m_data);
    }
    Value operator*(const Value& rhs) {
        if (m_data.index() != rhs.m_data.index()) {
            return Value();
        }

        return std::visit(overloaded{
                              [](const std::string&) { return Value(); },                                //
                              [&](auto arg) { return Value(arg * std::get<decltype(arg)>(rhs.m_data)); } //
                          },
                          m_data);
    }

    Value operator/(const Value& rhs) {
        if (m_data.index() != rhs.m_data.index()) {
            return Value();
        }

        return std::visit(overloaded{
                              [](const std::string&) { return Value(); },                                //
                              [&](auto arg) { return Value(arg / std::get<decltype(arg)>(rhs.m_data)); } //
                          },
                          m_data);
    }

    std::string to_string() const {
        return std::visit(overloaded{
                              [](int64_t arg) { return fmt::format("{}", arg); },               //
                              [](double arg) { return fmt::format("{}", arg); },                //
                              [](const std::string& arg) { return fmt::format("\"{}\"", arg); } //
                          },
                          m_data);
    }
    bool operator==(const Value& rhs) const {
        if (m_data.index() != rhs.m_data.index()) {
            return false;
        }
        assert(m_data.index() == 0);
        return std::get<0>(m_data) == std::get<0>(rhs.m_data);
    }

private:
    std::variant<int64_t, double, std::string> m_data;
};

inline std::ostream& operator<<(std::ostream& os, const Value& value) {
    os << value.to_string();
    return os;
}

} // namespace asc

namespace fmt {
template <>
struct formatter<asc::Value> : formatter<string_view> {
    template <typename FormatContext>
    auto format(const asc::Value& value, FormatContext& ctx) {
        return formatter<string_view>::format(value.to_string(), ctx);
    }
};

} // namespace fmt
