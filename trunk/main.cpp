#include <iostream>
#include <fstream>
#include <cstring>
#include "utils.h"
#include "largeint.h"

int main(int argc, char ** argv)
{
    LargeInteger li1(10);
    LargeInteger li2(-10);

    std::cout << li1 << "\n"
            << li2 << "\n"
            << (li1 + li2) << "\n"
            << li1 * li2 << "\n"
            << li1 - li2 << std::endl;
    proccess_files("input.txt", "output.txt");
}

