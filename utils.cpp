#include <fstream>
#include "utils.h"
#include "largeint.h"

void eval_expr(LargeInteger & n1, LargeInteger & n2, char op)
{
    if (op == '+')
        n1.add(n2);
    //if (op == '-')
    //    return n1 - n2;
    //if (op == '*')
    //    return n1 * n2;
    //if (op == '/')
    //    return n1 / n2;
    //return n1;
}

void proccess_files(char * input_filename, char * output_filename)
{
    std::ifstream input_stream(input_filename);

    std::ofstream output_stream(output_filename);

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

	n1.from_string(s1.c_str());
	n2.from_string(s2.c_str());
	eval_expr(n1, n2, op);

	char * s = n1.to_string();

        output_stream << s << "\n";
	
	delete [] s;

        --line_count;
    }
}
