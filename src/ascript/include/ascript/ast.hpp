#pragma once

#include <memory>
#include <sstream>
#include <vector>

#include <fmt/format.h>

#include "source_location.hpp"

namespace asc {

class Expression;

using Value = int64_t;

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
    Program(SourceLocation source_location, std::unique_ptr<Expression> expression)
        : ASTNode(source_location), m_expression(std::move(expression)) {}

    void dump(int indentation, std::stringstream& builder) const override;
    Value execute() const;

private:
    std::unique_ptr<Expression> m_expression;
};

class Expression : public ASTNode {
public:
    virtual Value execute() const = 0;

protected:
    explicit Expression(SourceLocation source_location) : ASTNode(source_location) {}
};

class Literal final : public Expression {
public:
    Literal(SourceLocation source_location, Value value) : Expression(source_location), m_value(std::move(value)) {}
    Value execute() const override { return m_value; }
    void dump(int indentation, std::stringstream& builder) const override;

private:
    Value m_value;
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

    Value execute() const override { return {}; }
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
