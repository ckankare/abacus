#include "ast.hpp"

#include <fmt/format.h>

#include "interpreter.hpp"

namespace asc {
static std::string whitespaces(int count) { return fmt::format("{:{}}", "", count); }

void FunctionDeclaration::dump(int indentation, std::stringstream& builder) const {
    builder << whitespaces(indentation) << fmt::format("Function '{}({})':\n", m_name, fmt::join(m_arguments, ", "));
    m_body->dump(indentation + 2, builder);
}

Value Program::execute(Interpreter& interpreter) const {
    if (m_expression) {
        return m_expression->execute(interpreter);
    }

    return Value{};
}

void Program::dump(int indentation, std::stringstream& builder) const {
    builder << whitespaces(indentation) << fmt::format("Program:\n");
    for (const auto& function : m_functions) {
        function->dump(indentation + 2, builder);
    }

    if (m_expression) {
        m_expression->dump(indentation + 2, builder);
    }
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

Value CallExpression::execute(Interpreter& interpreter) const {
    auto callee = interpreter.find_function(m_name);
    if (callee == nullptr) {
        throw new std::runtime_error(fmt::format("Function '{}' undefined!", m_name));
    }

    std::vector<Value> calculated_arguments;

    calculated_arguments.reserve(m_arguments.size());
    for (const auto& argument : m_arguments) {
        calculated_arguments.push_back(argument->execute(interpreter));
    }

    return callee->call(std::move(calculated_arguments));
}

void CallExpression::dump(int indentation, std::stringstream& builder) const {
    builder << whitespaces(indentation) << fmt::format("CallExpression: {}\n", m_name);
    for (const auto& argument : m_arguments) {
        argument->dump(indentation + 2, builder);
    }
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
