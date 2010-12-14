#ifndef EXPRESSION_H
#define EXPRESSION_H

class Function
{
public:
    virtual ~Function();
    
    virtual double eval(double x) const = 0;
};

class BinaryOperator : public Function
{
public:
    BinaryOperator(const Function* left, const Function* right);
    ~BinaryOperator();

    virtual double eval(double x) const = 0;
protected:
    const Function* getLeft() const;
    const Function* getRight() const;
private:
    const Function* m_left;
    const Function* m_right;

    BinaryOperator& operator = (const BinaryOperator& expr);
};

class Add : public BinaryOperator
{
public:
    Add(const Function* left, const Function* right);

    virtual double eval(double x) const;
};

class Subtract : public BinaryOperator
{
public:
    Subtract(const Function* left, const Function* right);

    virtual double eval(double x) const;
};

class Multiply : public BinaryOperator
{
public:
    Multiply(const Function* left, const Function* right);

    virtual double eval(double x) const;
};

class Divide : public BinaryOperator
{
public:
    Divide(const Function* left, const Function* right);

    virtual double eval(double x) const;
};

class Constant : public Function
{
public:
    explicit Constant(double value);

    double eval(double x) const;
private:
    double m_value;
};

class VariableExpr : public Function
{
public:
    double eval(double x) const;
};

#endif
