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

double integrate(double a, double b, double eps, const Function* expr)
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

double solve(double a, double b, double eps, const Function* expr, bool* solved)
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

const Function* buildBinaryExpression(const Function* left, const Function* right, char op)
{
    if (op == '+')
    {
        return new Add(left, right);
    }
    else if (op == '-')
    {
        return new Subtract(left, right);
    }
    else if (op == '*')
    {
        return new Multiply(left, right);
    }
    else if (op == '/')
    {
        return new Divide(left, right);
    }

    return 0;
}

const Function* buildExpression(const std::string& str)
{
    size_t p = str.find_first_of("+-");
    if (p == str.npos)
        p = str.find_first_of("*/");

    if (p != str.npos)
    {
        const Function* left;
        const Function* right;

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
        return new Constant(n);
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

        const Function* expr = buildExpression(exprStr);

        string cmd;
        inStream >> cmd;

        double a;
        double b;
        double eps;
        inStream >> a >> b >> eps;

        if (cmd == "sol")
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
