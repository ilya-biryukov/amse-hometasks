#include <iostream>
#include <fstream>
#include <cstring>
#include "utils.h"
#include "largeint.h"

int main(int argc, char ** argv)
{
    LargeInteger int1;
    LargeInteger int2;

    int1.from_string("1222");
    int2 = int1;
    std::cout << int1.to_string() << "\n" << int2.to_string();
    proccess_files("input.txt", "output.txt");
}
