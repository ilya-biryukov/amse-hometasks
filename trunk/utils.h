#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED
#include <string>
class LargeInteger;

void eval_expr(LargeInteger & n1, LargeInteger & n2, char op);
void proccess_files(const std::string & input_filename,
                    const std::string & output_filename);

#endif // UTILS_H_INCLUDED
