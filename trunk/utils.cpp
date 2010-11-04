#include <fstream>
#include "utils.h"

double eval_expr(double n1, double n2, char op)
{
    if (op == '+')
        return n1 + n2;
    if (op == '-')
        return n1 - n2;
    if (op == '*')
        return n1 * n2;
    if (op == '/')
        return n1 / n2;

    return n1;
}

void proccess_files(char * input_filename, char * output_filename)
{
    std::ifstream input_stream(input_filename);

    std::ofstream output_stream(output_filename);

    int line_count = 0;
    input_stream >> line_count;

    while (line_count)
    {
        double n1;
        double n2;
        char   op;

        input_stream >> n1 >> op >> n2;

        output_stream << eval_expr(n1, n2, op) << "\n";

        --line_count;
    }
}
