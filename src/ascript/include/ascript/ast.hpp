#pragma once

#include <memory>
#include <sstream>
#include <vector>

#include <fmt/format.h>

#include "source_location.hpp"
#include "value.hpp"

namespace asc {

class Expression;
class FunctionDeclaration;
class Interpreter;
class Statement;

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

class Program final : public ASTNode {
public:
    Program(SourceLocation source_location, std::unique_ptr<Statement> statement,
            std::vector<std::unique_ptr<FunctionDeclaration>> functions)
        : ASTNode(source_location), m_statement(std::move(statement)), m_functions(std::move(functions)) {}

    void dump(int indentation, std::stringstream& builder) const override;
    Value execute(Interpreter& interpreter) const;

private:
    std::unique_ptr<Statement> m_statement;
    std::vector<std::unique_ptr<FunctionDeclaration>> m_functions;
};

class FunctionDeclaration final : public ASTNode {
public:
    FunctionDeclaration(SourceLocation source_location, std::string name, std::vector<std::string> arguments,
                        std::unique_ptr<Statement> body)
        : ASTNode(source_location), m_name(std::move(name)), m_arguments(std::move(arguments)),
          m_body(std::move(body)) {}

    void dump(int indentation, std::stringstream& builder) const override;
    const std::string& name() const { return m_name; }
    const std::vector<std::string>& arguments() const { return m_arguments; }
    const Statement* body() const { return m_body.get(); }

private:
    std::string m_name;
    std::vector<std::string> m_arguments;
    std::unique_ptr<Statement> m_body;
};

class Statement : public ASTNode {
public:
    virtual void execute(Interpreter& interpreter) const = 0;

protected:
    Statement(SourceLocation source_location) : ASTNode(source_location) {}
};

class BlockStatement final : public Statement {
public:
    BlockStatement(SourceLocation source_location, std::vector<std::unique_ptr<Statement>> statements)
        : Statement(source_location), m_statements(std::move(statements)) {}
    void execute(Interpreter& interpreter) const override;
    void dump(int indentation, std::stringstream& builder) const override;

private:
    std::vector<std::unique_ptr<Statement>> m_statements;
};

class ReturnStatement final : public Statement {
public:
    ReturnStatement(SourceLocation source_location, std::unique_ptr<Expression> expression)
        : Statement(source_location), m_expression(std::move(expression)) {}

    void execute(Interpreter& interpreter) const override;
    void dump(int indentation, std::stringstream& builder) const override;

private:
    std::unique_ptr<Expression> m_expression;
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
    assert(false);
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
