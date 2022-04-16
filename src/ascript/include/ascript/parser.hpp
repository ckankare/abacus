#pragma once

#include <memory>

#include "lexer.hpp"
#include "scanner.hpp"

namespace asc {
class Program;
class Expression;
class BinaryExpression;

class Parser {
public:
    explicit Parser(std::string_view source) : m_scanner(source), m_lexer(m_scanner) {}
    std::unique_ptr<Program> parse();

protected:
    std::unique_ptr<Expression> parse_expression(uint32_t precedence);
    std::unique_ptr<Expression> parse_primary_expression();

private:
    Scanner m_scanner;
    Lexer m_lexer;
};
} // namespace asc
