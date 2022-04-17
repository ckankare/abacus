#include "ast.hpp"

#include <fmt/format.h>

namespace asc {
static std::string whitespaces(int count) { return fmt::format("{:{}}", "", count); }

void Program::dump(int indentation, std::stringstream& builder) const {
    builder << whitespaces(indentation) << fmt::format("Program:\n");
    m_expression->dump(indentation + 2, builder);
}

Value Program::execute() const { return m_expression->execute(); }

void Literal::dump(int indentation, std::stringstream& builder) const {
    builder << whitespaces(indentation) << fmt::format("Literal: {}\n", m_value);
}

void BinaryExpression::dump(int indentation, std::stringstream& builder) const {
    builder << whitespaces(indentation) << fmt::format("BinaryExpression: {}\n", m_op);
    m_lhs->dump(indentation + 2, builder);
    m_rhs->dump(indentation + 2, builder);
}

Value BinaryExpression::execute() const {
    switch (m_op) {
        case BinaryOp::Addition: return m_lhs->execute() + m_rhs->execute();
        case BinaryOp::Subtraction: return m_lhs->execute() - m_rhs->execute();
        case BinaryOp::Multiplication: return m_lhs->execute() * m_rhs->execute();
        case BinaryOp::Division: return m_lhs->execute() / m_rhs->execute();
    }
}

} // namespace asc
