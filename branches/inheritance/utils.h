#ifndef UTILS_H
#define UTILS_H

#include <string>
#include "expression.h"

double solve(double a, double b, double eps, const Expression* expr, bool* solved = 0);

double integrate(double a, double b, double eps, const Expression* expr);

const Expression* buildExpression(const std::string& str);

void proccessFiles(const std::string& inputFilename,
                   const std::string& outputFilename);


#endif // UTILS_H
