#include <fstream>
#include "utils.h"

int getDoubleSign(double x)
{
    if (x < 0.)
        return -1;
    else if (x == 0.)
        return 0;
    else
        return 1;
}

double integrate(double a, double b, double eps, const Expression* expr)
{
    double x = a;
    double res = 0.;
    while (x < b)
    {
        res += expr->eval(x) * eps;
        x += eps;
    }

    return res;
}

double solve(double a, double b, double eps, const Expression* expr, bool* solved)
{
    if (solved)
        *solved = true;
    while ((b - a) > eps)
    {
        int aSign = getDoubleSign(expr->eval(a));
        int bSign = getDoubleSign(expr->eval(b));

        if (aSign == 0)
        {
            return a;
        }
        else if (bSign == 0)
        {
            return b;
        }
        else if (aSign == bSign)
        {
            if (solved)
                *solved = false;
            return 0.;
        }
        else
        {
            double mid = (a + b) / 2.;
            int midSign = getDoubleSign(expr->eval(mid));

            if (midSign == 0)
                return mid;

            if (midSign == aSign)
            {
                a = mid;
            }
            else
            {
                b = mid;
            }
        }
    }

    return a;
}

const Expression* buildBinaryExpression(const Expression* left, const Expression* right, char op)
{
    if (op == '+')
    {
        return new PlusExpr(left, right);
    }
    else if (op == '-')
    {
        return new MinusExpr(left, right);
    }
    else if (op == '*')
    {
        return new MulExpr(left, right);
    }
    else if (op == '/')
    {
        return new DivExpr(left, right);
    }

    return 0;
}

const Expression* buildExpression(const std::string& str)
{
    size_t p = str.find_first_of("+-");
    if (p == str.npos)
        p = str.find_first_of("*/");

    if (p != str.npos)
    {
        const Expression* left;
        const Expression* right;

        left = buildExpression(str.substr(0, p));
        right = buildExpression(str.substr(p+1));
        return buildBinaryExpression(left, right, str[p]);
    }

    if (str[0] == 'x')
    {
        return new VariableExpr();
    }
    else if ((str[0] >= '0') && (str[0] <= '9') || (str[0] =='.'))
    {
        double n;
        sscanf(str.c_str(), "%lf", &n);
        return new ConstantExpr(n);
    }

    return 0;
}

void proccessFiles(const std::string& inputFilename,
                   const std::string& outputFilename)
{
    using namespace std;

    ifstream inStream(inputFilename.c_str());
    ofstream outStream(outputFilename.c_str());

    size_t lineCount;
    inStream >> lineCount;

    while (lineCount--)
    {
        string exprStr;
        inStream >> exprStr;

        const Expression* expr = buildExpression(exprStr);

        string cmd;
        inStream >> cmd;

        double a;
        double b;
        double eps;
        inStream >> a >> b >> eps;

        if (cmd == "solve")
        {
            bool solved;
            double root = solve(a, b, eps, expr, &solved);

            if (solved)
                outStream << root;
            else
                outStream << "no solution";
	    outStream << "\n";
        }
        else if (cmd == "int")
        {
            outStream << integrate(a, b, eps, expr) << "\n";
        }

        delete expr;
    }
}
