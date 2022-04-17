#include "ast.hpp"

#include <fmt/format.h>

#include "interpreter.hpp"

namespace asc {
static std::string whitespaces(int count) { return fmt::format("{:{}}", "", count); }

Value Program::execute(Interpreter& interpreter) const { return m_expression->execute(interpreter); }

void Program::dump(int indentation, std::stringstream& builder) const {
    builder << whitespaces(indentation) << fmt::format("Program:\n");
    m_expression->dump(indentation + 2, builder);
}

void Literal::dump(int indentation, std::stringstream& builder) const {
    builder << whitespaces(indentation) << fmt::format("Literal: {}\n", m_value);
}

Value Identifier::execute(Interpreter& interpreter) const {
    auto variable = interpreter.find_variable(m_name);
    if (variable == nullptr) {
        throw new std::runtime_error(fmt::format("Variable '{}' undefined!", m_name));
    }

    return *variable;
}

void Identifier::dump(int indentation, std::stringstream& builder) const {
    builder << whitespaces(indentation) << fmt::format("Identifier: {}\n", m_name);
}

Value BinaryExpression::execute(Interpreter& interpreter) const {
    switch (m_op) {
        case BinaryOp::Addition: return m_lhs->execute(interpreter) + m_rhs->execute(interpreter);
        case BinaryOp::Subtraction: return m_lhs->execute(interpreter) - m_rhs->execute(interpreter);
        case BinaryOp::Multiplication: return m_lhs->execute(interpreter) * m_rhs->execute(interpreter);
        case BinaryOp::Division: return m_lhs->execute(interpreter) / m_rhs->execute(interpreter);
    }
}

void BinaryExpression::dump(int indentation, std::stringstream& builder) const {
    builder << whitespaces(indentation) << fmt::format("BinaryExpression: {}\n", m_op);
    m_lhs->dump(indentation + 2, builder);
    m_rhs->dump(indentation + 2, builder);
}

} // namespace asc
