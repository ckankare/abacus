#pragma once

#include <algorithm>

#include <common/meta.hpp>

namespace asc {
enum class TokenType {
    If,
    Then,
    Else,
    Return,
    Function,

    Assign,
    Literal,
    Identifier,
    StringLiteral,

    Equals,
    Plus,
    Minus,
    Multiplication,
    Division,
    Less,
    LessEqual,
    Greater,
    GreaterEqual,
    Exclamation,

    Comma,
    Semicolon,
    At,
    DollarSign,

    LeftCurly,
    RightCurly,
    LeftParenthesis,
    RightParenthesis,

    RightArrow,

    And,
    Or,
    BitwiseAnd,
    BitwiseOr,

    EndOfStream,
};

struct Token {
public:
    Token() : m_type(TokenType::EndOfStream) {}
    explicit Token(TokenType type) : m_type(type) {}
    Token(TokenType type, std::string value) : m_type(type), m_value(std::move(value)) {}

    friend bool operator==(const Token& lhs, const Token& rhs) {
        return lhs.m_type == rhs.m_type && lhs.m_value == rhs.m_value;
    }

    constexpr bool is_end() const { return m_type == TokenType::EndOfStream; }

    template <typename... Args>
    constexpr bool is(Args&&... args) const {
        static_assert(utils::are_same<TokenType, Args...>::value, "All must be TokenType");
        const auto array = utils::make_array(std::forward<Args>(args)...);
        return std::any_of(array.begin(), array.end(), [this](auto v) { return v == m_type; });
    }

    constexpr TokenType type() const { return m_type; }
    std::string_view value() const { return m_value; }

private:
    TokenType m_type;
    std::string m_value;
};
} // namespace asc
