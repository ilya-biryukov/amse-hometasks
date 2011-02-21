#include <iostream>
#include "singleton.h"

int main(int argc, char * argv[])
{
    Singleton & s = Singleton::instance();
    std::cout << s.getTen() << std::endl;
    return 0;
}
