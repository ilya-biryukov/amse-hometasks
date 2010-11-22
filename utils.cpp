#include <iostream>
#include <fstream>
#include "utils.h"
#include "largeint.h"

LargeInteger eval_expr(LargeInteger & n1, LargeInteger & n2, char op)
{
    if (op == '+')
        return n1 + n2;
    if (op == '-')
        return n1 - n2;
    if (op == '*')
        return n1 * n2;

    return LargeInteger(n1);
}

void proccess_files(const std::string & input_filename,
                    const std::string & output_filename)
{
    std::ifstream input_stream(input_filename.c_str());
    std::ofstream output_stream(output_filename.c_str());

    int line_count = 0;
    input_stream >> line_count;

    while (line_count)
    {
	LargeInteger n1;
	LargeInteger n2;
        char   op;

        input_stream >> n1 >> op >> n2;
        output_stream << eval_expr(n1, n2, op) << "\n";

        --line_count;
    }
}
