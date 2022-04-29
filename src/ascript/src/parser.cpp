#include "parser.hpp"

#include <cassert>
#include <charconv>

#include "ast.hpp"

namespace asc {

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
    throw new std::runtime_error("Invalid operator!");
}

static BinaryOp to_binary_op(TokenType token_type) {
    switch (token_type) {
        case TokenType::Plus: return BinaryOp::Addition;
        case TokenType::Minus: return BinaryOp::Subtraction;
        case TokenType::Asterisk: return BinaryOp::Multiplication;
        case TokenType::Slash: return BinaryOp::Division;
        default: throw new std::runtime_error("Invalid binary token!");
    }
}
std::unique_ptr<Program> Parser::parse() {
    auto expression = parse_expression(0);
    return std::make_unique<Program>(SourceLocation{}, std::move(expression));
}

std::unique_ptr<Expression> Parser::parse_expression(uint32_t precedence) {
    auto expression = parse_primary_expression();

    while (m_lexer.look_ahead().is(binary_operation_tokens)) {

        auto op = m_lexer.look_ahead();
        if (!op.is(binary_operation_tokens)) {
            throw new std::runtime_error("Couldn't parse expression!");
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
    auto next = m_lexer.next();
    assert(next.type() != TokenType::EndOfStream);

    switch (next.type()) {
        case TokenType::LeftParenthesis: {
            auto expression = parse_expression(0);
            if (m_lexer.next().type() != TokenType::RightParenthesis) {
                throw new std::runtime_error("Missing right parenthesis!");
            }
            return expression;
        }
        case TokenType::Literal: {
            int64_t value;
            auto result = std::from_chars(next.value().data(), next.value().data() + next.value().size(), value);
            (void)result; // TODO
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
                        throw new std::runtime_error("Expecting comma or right parenthesis!");
                    }
                    if (m_lexer.look_ahead().type() == TokenType::Comma) {
                        m_lexer.consume();
                    }
                }

                if (m_lexer.next().type() != TokenType::RightParenthesis) {
                    throw new std::runtime_error("Missing right parenthesis!");
                }
                return std::make_unique<CallExpression>(SourceLocation{}, std::move(name), std::move(arguments));
            } else {
                return std::make_unique<Identifier>(SourceLocation{}, std::string(next.value()));
            }
        }
        default: throw new std::runtime_error("Invalid token for primary expression!");
    }
}

} // namespace asc
