#include "expression.h"

Expression::~Expression()
{}

BinaryOpExpr::BinaryOpExpr(const Expression* left, const Expression* right)
    : m_left(left), m_right(right)
{}

BinaryOpExpr::~BinaryOpExpr()
{
    delete m_left;
    delete m_right;
}

const Expression* BinaryOpExpr::getLeft() const
{
    return m_left;
}

const Expression* BinaryOpExpr::getRight() const
{
    return m_right;
}

PlusExpr::PlusExpr(const Expression* left, const Expression* right)
    : BinaryOpExpr(left, right)
{}

double PlusExpr::eval(double x) const
{
    return getLeft()->eval(x) + getRight()->eval(x);
}

MinusExpr::MinusExpr(const Expression* left, const Expression* right)
    : BinaryOpExpr(left, right)
{}

double MinusExpr::eval(double x) const
{
    return getLeft()->eval(x) - getRight()->eval(x);
}

MulExpr::MulExpr(const Expression* left, const Expression* right)
    : BinaryOpExpr(left, right)
{}

double MulExpr::eval(double x) const
{
    return getLeft()->eval(x) * getRight()->eval(x);
}

DivExpr::DivExpr(const Expression* left, const Expression* right)
    : BinaryOpExpr(left, right)
{}

double DivExpr::eval(double x) const
{
    return getLeft()->eval(x) / getRight()->eval(x);
}

ConstantExpr::ConstantExpr(double value)
    : m_value(value)
{}

double ConstantExpr::eval(double x) const
{
    return m_value;
}

double VariableExpr::eval(double x) const
{
    return x;
}
