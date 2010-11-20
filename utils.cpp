#include <iostream>
#include <fstream>
#include "utils.h"
#include "largeint.h"

void eval_expr(LargeInteger & n1, LargeInteger & n2, char op)
{
    if (op == '+')
        n1.add(n2);
    if (op == '-')
        n1.sub(n2);
    if (op == '*')
        n1.mul(n2);
    //if (op == '/')
    //    return n1 / n2;
    //return n1;
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
	std::string s1;
	std::string s2;
        char   op;

        input_stream >> s1 >> op >> s2;

	LargeInteger n1;
	LargeInteger n2;

        n1.from_string(s1);
        n2.from_string(s2);

	eval_expr(n1, n2, op);

        std::string s = n1.to_string();

        output_stream << s << "\n";
        --line_count;
    }
}
