#ifndef EXPRESSION_H
#define EXPRESSION_H

class Expression
{
public:
    virtual ~Expression();
    
    virtual double eval(double x) const = 0;
};

class BinaryOpExpr : public Expression
{
public:
    BinaryOpExpr(const Expression* left, const Expression* right);
    ~BinaryOpExpr();

    virtual double eval(double x) const = 0;
protected:
    const Expression* getLeft() const;
    const Expression* getRight() const;
private:
    const Expression* m_left;
    const Expression* m_right;

    BinaryOpExpr& operator = (const BinaryOpExpr& expr);
};

class PlusExpr : public BinaryOpExpr
{
public:
    PlusExpr(const Expression* left, const Expression* right);

    virtual double eval(double x) const;
};

class MinusExpr : public BinaryOpExpr
{
public:
    MinusExpr(const Expression* left, const Expression* right);

    virtual double eval(double x) const;
};

class MulExpr : public BinaryOpExpr
{
public:
    MulExpr(const Expression* left, const Expression* right);

    virtual double eval(double x) const;
};

class DivExpr : public BinaryOpExpr
{
public:
    DivExpr(const Expression* left, const Expression* right);

    virtual double eval(double x) const;
};

class ConstantExpr : public Expression
{
public:
    explicit ConstantExpr(double value);

    double eval(double x) const;
private:
    double m_value;
};

class VariableExpr : public Expression
{
public:
    double eval(double x) const;
};

#endif
