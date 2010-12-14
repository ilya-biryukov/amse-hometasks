#ifndef UTILS_H
#define UTILS_H

#include <string>
#include "function.h"

double solve(double a, double b, double eps, const Function* expr, bool* solved = 0);

double integrate(double a, double b, double eps, const Function* expr);

const Function* buildExpression(const std::string& str);

void proccessFiles(const std::string& inputFilename,
                   const std::string& outputFilename);


#endif // UTILS_H
