#include "function.h"

Function::~Function()
{}

BinaryOperator::BinaryOperator(const Function* left, const Function* right)
    : m_left(left), m_right(right)
{}

BinaryOperator::~BinaryOperator()
{
    delete m_left;
    delete m_right;
}

const Function* BinaryOperator::getLeft() const
{
    return m_left;
}

const Function* BinaryOperator::getRight() const
{
    return m_right;
}

Add::Add(const Function* left, const Function* right)
    : BinaryOperator(left, right)
{}

double Add::eval(double x) const
{
    return getLeft()->eval(x) + getRight()->eval(x);
}

Subtract::Subtract(const Function* left, const Function* right)
    : BinaryOperator(left, right)
{}

double Subtract::eval(double x) const
{
    return getLeft()->eval(x) - getRight()->eval(x);
}

Multiply::Multiply(const Function* left, const Function* right)
    : BinaryOperator(left, right)
{}

double Multiply::eval(double x) const
{
    return getLeft()->eval(x) * getRight()->eval(x);
}

Divide::Divide(const Function* left, const Function* right)
    : BinaryOperator(left, right)
{}

double Divide::eval(double x) const
{
    return getLeft()->eval(x) / getRight()->eval(x);
}

Constant::Constant(double value)
    : m_value(value)
{}

double Constant::eval(double x) const
{
    return m_value;
}

double VariableExpr::eval(double x) const
{
    return x;
}
