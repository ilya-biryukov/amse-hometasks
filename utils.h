#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED
#include <string>
class LargeInteger;

LargeInteger eval_expr(const LargeInteger & n1, const LargeInteger & n2, char op);
void proccess_files(const std::string & input_filename,
                    const std::string & output_filename);

#endif // UTILS_H_INCLUDED
