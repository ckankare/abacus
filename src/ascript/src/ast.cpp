#include "ast.hpp"

#include <fmt/format.h>

#include "interpreter.hpp"

namespace asc {
static std::string whitespaces(int count) { return fmt::format("{:{}}", "", count); }

class ScriptFunction final : public Function {
public:
    explicit ScriptFunction(const FunctionDeclaration& declaration) : m_declaration(&declaration) {}
    Value call(std::vector<Value> arguments) override {
        Interpreter interpreter;

        auto decl_arguments = m_declaration->arguments();
        if (arguments.size() != decl_arguments.size()) {
            throw std::runtime_error(
                fmt::format("Expected {} arguments, {} provided!", decl_arguments.size(), arguments.size()));
        }

        for (std::size_t i = 0; i < arguments.size(); ++i) {
            interpreter.define_variable(decl_arguments[i], arguments[i]);
        }

        m_declaration->body()->execute(interpreter);
        return interpreter.result();
    }

    const FunctionDeclaration* m_declaration;
};

Value Program::execute(Interpreter& interpreter) const {
    for (const auto& function : m_functions) {
        auto script_function = std::make_unique<ScriptFunction>(*function);
        interpreter.define_function(function->name(), std::move(script_function));
    }

    if (m_statement) {
        m_statement->execute(interpreter);
    }

    return interpreter.result();
}

void Program::dump(int indentation, std::stringstream& builder) const {
    builder << whitespaces(indentation) << fmt::format("Program:\n");
    for (const auto& function : m_functions) {
        function->dump(indentation + 2, builder);
    }

    if (m_statement) {
        m_statement->dump(indentation + 2, builder);
    }
}

void FunctionDeclaration::dump(int indentation, std::stringstream& builder) const {
    builder << whitespaces(indentation) << fmt::format("Function '{}({})':\n", m_name, fmt::join(m_arguments, ", "));
    m_body->dump(indentation + 2, builder);
}

void BlockStatement::dump(int indentation, std::stringstream& builder) const {
    builder << whitespaces(indentation) << fmt::format("Block:\n");
    for (const auto& statement : m_statements) {
        statement->dump(indentation + 2, builder);
    }
}

void BlockStatement::execute(Interpreter& interpreter) const {
    for (const auto& statement : m_statements) {
        statement->execute(interpreter);
    }
}

void ReturnStatement::dump(int indentation, std::stringstream& builder) const {
    builder << whitespaces(indentation) << fmt::format("Return:\n");
    if (m_expression) {
        m_expression->dump(indentation + 2, builder);
    }
}

void ReturnStatement::execute(Interpreter& interpreter) const {
    if (m_expression) {
        auto value = m_expression->execute(interpreter);
        interpreter.set_result(std::move(value));
    }
}

void AssignStatement::dump(int indentation, std::stringstream& builder) const {
    builder << whitespaces(indentation) << fmt::format("Assignment: {}\n", m_name);
    m_expression->dump(indentation + 2, builder);
}

void AssignStatement::execute(Interpreter& interpreter) const {
    auto* variable = interpreter.find_variable(m_name);
    auto value = m_expression->execute(interpreter);
    if (variable) {
        *variable = value;
    } else {
        interpreter.define_variable(m_name, std::move(value));
    }
}

void Literal::dump(int indentation, std::stringstream& builder) const {
    builder << whitespaces(indentation) << fmt::format("Literal: {}\n", m_value);
}

Value Identifier::execute(Interpreter& interpreter) const {
    auto variable = interpreter.find_variable(m_name);
    if (variable == nullptr) {
        throw std::runtime_error(fmt::format("Variable '{}' undefined!", m_name));
    }

    return *variable;
}

void Identifier::dump(int indentation, std::stringstream& builder) const {
    builder << whitespaces(indentation) << fmt::format("Identifier: {}\n", m_name);
}

Value CallExpression::execute(Interpreter& interpreter) const {
    auto callee = interpreter.find_function(m_name);
    if (callee == nullptr) {
        throw std::runtime_error(fmt::format("Function '{}' undefined!", m_name));
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
    assert(false);
}

void BinaryExpression::dump(int indentation, std::stringstream& builder) const {
    builder << whitespaces(indentation) << fmt::format("BinaryExpression: {}\n", m_op);
    m_lhs->dump(indentation + 2, builder);
    m_rhs->dump(indentation + 2, builder);
}

} // namespace asc
