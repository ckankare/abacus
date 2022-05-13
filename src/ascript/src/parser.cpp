#include "parser.hpp"

#include <cassert>
#include <charconv>

#include "ast.hpp"

namespace asc {

Value from_str(std::string_view str, uint32_t base) {
    auto to_value = [](auto ec, auto value) {
        if (ec == std::errc::invalid_argument) {
            throw std::runtime_error("Invalid literal literal!");
        } else if (ec == std::errc::result_out_of_range) {
            throw std::runtime_error("Literal out-of-range!");
        }
        return Value(value);
    };

    switch (std::count(str.begin(), str.end(), '.')) {
        case 0: {
            int64_t value;
            auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), value, base);
            return to_value(ec, value);
        }
        case 1: {
            if (base != 10) {
                throw std::runtime_error("Only base 10 supported for real numbers!");
            }
            double value;
            auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), value, std::chars_format::fixed);
            return to_value(ec, value);
        }
        default: throw std::runtime_error("Expected 0 or 1 '.' in literal!");
    }
}

enum class Associative {
    None,
    Left,
    Right,
};

struct OperatorPrecedence {
    TokenType type;
    uint32_t precedence;
    Associative associative;
};

static constexpr std::array<OperatorPrecedence, 4> operator_table = {{
    {TokenType::Plus, 3, Associative::Left},
    {TokenType::Minus, 3, Associative::Left},
    {TokenType::Asterisk, 5, Associative::Left},
    {TokenType::Slash, 5, Associative::Left},
}};

static constexpr auto binary_operation_tokens =
    utils::make_array(TokenType::Plus, TokenType::Minus, TokenType::Asterisk, TokenType::Slash);

static OperatorPrecedence get_precedence(TokenType type) {
    for (const auto& it : operator_table) {
        if (it.type == type) {
            return it;
        }
    }
    throw std::runtime_error("Invalid operator!");
}

static BinaryOp to_binary_op(TokenType token_type) {
    switch (token_type) {
        case TokenType::Plus: return BinaryOp::Addition;
        case TokenType::Minus: return BinaryOp::Subtraction;
        case TokenType::Asterisk: return BinaryOp::Multiplication;
        case TokenType::Slash: return BinaryOp::Division;
        default: throw std::runtime_error("Invalid binary token!");
    }
}
std::unique_ptr<Program> Parser::parse() {
    std::vector<std::unique_ptr<FunctionDeclaration>> functions;
    while (m_lexer.look_ahead().type() == TokenType::Function) {
        functions.push_back(parse_function_declaration());
    }

    std::unique_ptr<Expression> expression;
    if (m_lexer.look_ahead().type() != TokenType::EndOfStream) {
        expression = parse_expression(0);
    }
    return std::make_unique<Program>(SourceLocation{}, std::move(expression), std::move(functions));
}

void Parser::require_consume(TokenType token_type) {
    auto next = m_lexer.next();
    if (next.type() != token_type) {
        throw std::runtime_error(fmt::format("Expected '{}' got '{}'!", token_type, next));
    }
}

Token Parser::require(TokenType token_type) {
    auto next = m_lexer.next();
    if (next.type() != token_type) {
        throw std::runtime_error(fmt::format("Expected '{}' got '{}'!", token_type, next));
    }
    return next;
}

std::unique_ptr<FunctionDeclaration> Parser::parse_function_declaration() {
    require_consume(TokenType::Function);
    auto identifier = require(TokenType::Identifier);
    require_consume(TokenType::LeftParenthesis);

    std::vector<std::string> arguments;
    while (!m_lexer.look_ahead().is(TokenType::RightParenthesis, TokenType::EndOfStream)) {
        arguments.push_back(std::string(require(TokenType::Identifier).value()));
        if (!m_lexer.look_ahead().is(TokenType::Comma, TokenType::RightParenthesis)) {
            throw std::runtime_error("Expecting comma or right parenthesis!");
        }
        if (m_lexer.look_ahead().type() == TokenType::Comma) {
            m_lexer.consume();
        }
    }
    require_consume(TokenType::RightParenthesis);

    require_consume(TokenType::LeftCurly);
    auto expression = parse_expression(0);
    require_consume(TokenType::RightCurly);
    return std::make_unique<FunctionDeclaration>(SourceLocation{}, std::string(identifier.value()),
                                                 std::move(arguments), std::move(expression));
}

std::unique_ptr<Expression> Parser::parse_expression(uint32_t precedence) {
    auto expression = parse_primary_expression();

    while (m_lexer.look_ahead().is(binary_operation_tokens)) {

        auto op = m_lexer.look_ahead();
        if (!op.is(binary_operation_tokens)) {
            throw std::runtime_error("Couldn't parse expression!");
        }

        auto op_precedence = get_precedence(op.type());
        if (op_precedence.precedence < precedence) {
            break;
        }
        m_lexer.consume();

        auto current_precedence = op_precedence.precedence;
        if (op_precedence.associative == Associative::Left) {
            current_precedence += 1;
        }

        auto rhs = parse_expression(current_precedence);
        expression = std::make_unique<BinaryExpression>(SourceLocation{}, to_binary_op(op.type()),
                                                        std::move(expression), std::move(rhs));
    }

    return expression;
}

std::unique_ptr<Expression> Parser::parse_primary_expression() {
    auto remove_prefix = [](std::string_view view, std::size_t n) {
        view.remove_prefix(n);
        return view;
    };

    auto next = m_lexer.next();
    assert(next.type() != TokenType::EndOfStream);

    switch (next.type()) {
        case TokenType::LeftParenthesis: {
            auto expression = parse_expression(0);
            if (m_lexer.next().type() != TokenType::RightParenthesis) {
                throw std::runtime_error("Missing right parenthesis!");
            }
            return expression;
        }
        case TokenType::Literal: {
            Value value;
            if (next.value().starts_with("0x")) {
                value = from_str(remove_prefix(next.value(), 2), 16);
            } else if (next.value().starts_with("0o")) {
                value = from_str(remove_prefix(next.value(), 2), 8);
            } else if (next.value().starts_with("0b")) {
                value = from_str(remove_prefix(next.value(), 2), 2);
            } else {
                value = from_str(next.value(), 10);
            }

            return std::make_unique<Literal>(SourceLocation{}, Value(value));
        }
        case TokenType::Identifier: {
            if (m_lexer.look_ahead().type() == TokenType::LeftParenthesis) {
                auto name = std::string(next.value());
                m_lexer.consume();
                std::vector<std::unique_ptr<Expression>> arguments;

                while (!m_lexer.look_ahead().is(TokenType::RightParenthesis, TokenType::EndOfStream)) {
                    arguments.push_back(std::move(parse_expression(0)));
                    if (!m_lexer.look_ahead().is(TokenType::Comma, TokenType::RightParenthesis)) {
                        throw std::runtime_error("Expecting comma or right parenthesis!");
                    }
                    if (m_lexer.look_ahead().type() == TokenType::Comma) {
                        m_lexer.consume();
                    }
                }

                if (m_lexer.next().type() != TokenType::RightParenthesis) {
                    throw std::runtime_error("Missing right parenthesis!");
                }
                return std::make_unique<CallExpression>(SourceLocation{}, std::move(name), std::move(arguments));
            } else {
                return std::make_unique<Identifier>(SourceLocation{}, std::string(next.value()));
            }
        }
        default: throw std::runtime_error("Invalid token for primary expression!");
    }
}

} // namespace asc
