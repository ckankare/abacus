#pragma once

#include <memory>
#include <sstream>
#include <vector>

#include <fmt/format.h>

#include "source_location.hpp"
#include "value.hpp"

namespace asc {

class Expression;
class Interpreter;

class ASTNode {
public:
    virtual ~ASTNode() = default;

    virtual void dump(int indentation, std::stringstream& builder) const = 0;
    SourceLocation source_location() const { return m_source_location; }

protected:
    explicit ASTNode(SourceLocation source_location) : m_source_location(source_location) {}

private:
    SourceLocation m_source_location;
};

class FunctionDeclaration final : public ASTNode {
public:
    FunctionDeclaration(SourceLocation source_location, std::string name, std::vector<std::string> arguments,
                        std::unique_ptr<Expression> body)
        : ASTNode(source_location), m_name(std::move(name)), m_arguments(std::move(arguments)),
          m_body(std::move(body)) {}

    void dump(int indentation, std::stringstream& builder) const override;

private:
    std::string m_name;
    std::vector<std::string> m_arguments;
    std::unique_ptr<Expression> m_body;
};

class Program final : public ASTNode {
public:
    Program(SourceLocation source_location, std::unique_ptr<Expression> expression,
            std::vector<std::unique_ptr<FunctionDeclaration>> functions)
        : ASTNode(source_location), m_expression(std::move(expression)), m_functions(std::move(functions)) {}

    void dump(int indentation, std::stringstream& builder) const override;
    Value execute(Interpreter& interpreter) const;

private:
    std::unique_ptr<Expression> m_expression;
    std::vector<std::unique_ptr<FunctionDeclaration>> m_functions;
};

class Expression : public ASTNode {
public:
    virtual Value execute(Interpreter& interpreter) const = 0;

protected:
    explicit Expression(SourceLocation source_location) : ASTNode(source_location) {}
};

class Literal final : public Expression {
public:
    Literal(SourceLocation source_location, Value value) : Expression(source_location), m_value(std::move(value)) {}
    Value execute(Interpreter&) const override { return m_value; }
    void dump(int indentation, std::stringstream& builder) const override;

private:
    Value m_value;
};

class Identifier final : public Expression {
public:
    Identifier(SourceLocation source_location, std::string name)
        : Expression(source_location), m_name(std::move(name)) {}
    Value execute(Interpreter& interpreter) const override;
    void dump(int indentation, std::stringstream& builder) const override;

private:
    std::string m_name;
};

class CallExpression final : public Expression {
public:
    CallExpression(SourceLocation source_location, std::string name, std::vector<std::unique_ptr<Expression>> arguments)
        : Expression(source_location), m_name(std::move(name)), m_arguments(std::move(arguments)) {}

    Value execute(Interpreter& interpreter) const override;
    void dump(int indentation, std::stringstream& builder) const override;

private:
    std::string m_name;
    std::vector<std::unique_ptr<Expression>> m_arguments;
};

enum class BinaryOp {
    Addition,
    Subtraction,
    Multiplication,
    Division,
};

constexpr std::string_view to_string(BinaryOp value) {
    switch (value) {
        case asc::BinaryOp::Addition: return "+";
        case asc::BinaryOp::Subtraction: return "-";
        case asc::BinaryOp::Multiplication: return "*";
        case asc::BinaryOp::Division: return "/";
    }
}

class BinaryExpression final : public Expression {
public:
    BinaryExpression(SourceLocation source_location, BinaryOp op, std::unique_ptr<Expression> lhs,
                     std::unique_ptr<Expression> rhs)
        : Expression(source_location), m_op(op), m_lhs(std::move(lhs)), m_rhs(std::move(rhs)) {}

    Value execute(Interpreter& interpreter) const override;
    void dump(int indentation, std::stringstream& builder) const override;

private:
    BinaryOp m_op;
    std::unique_ptr<Expression> m_lhs;
    std::unique_ptr<Expression> m_rhs;
};

} // namespace asc

namespace fmt {
template <>
struct formatter<asc::BinaryOp> : formatter<string_view> {
    template <typename FormatContext>
    auto format(asc::BinaryOp op, FormatContext& ctx) {
        return formatter<string_view>::format(asc::to_string(op), ctx);
    }
};

} // namespace fmt
