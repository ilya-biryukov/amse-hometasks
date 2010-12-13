#include <iostream>

class A
{
public:
    A();
    virtual int f() = 0;
};

class B : public A
{
public:
    virtual int f();
};

A::A()
{
    int i = ((B*)this)->f();
}

int B::f()
{
    return 10;
}

int main(int argc, char** argv)
{
    B tmp;

    return 0;
}
