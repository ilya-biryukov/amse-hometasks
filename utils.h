#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED
struct LargeInteger;

void eval_expr(LargeInteger & n1, LargeInteger & n2, char op);
void proccess_files(char * input_filename, char * output_filename);

#endif // UTILS_H_INCLUDED
