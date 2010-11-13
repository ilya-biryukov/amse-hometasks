#ifndef LARGEINT_H
#define LARGEINT_H

#include <string>

const unsigned int LV_BASE = 1000000000;
const size_t LV_DIGITCHARS = 9;
const size_t LV_INITSIZE   = 1;

struct LargeInteger
{
    explicit LargeInteger();
    LargeInteger(LargeInteger & src);
    ~LargeInteger();

    LargeInteger & operator=(const LargeInteger& li);

    unsigned int *  value;
    size_t          value_maxsize;
    size_t          value_size;

    void    add(LargeInteger & num);
    void    ensure_size(size_t size);
    void    update_size();
    char *  to_string();
    void    print_val();
    void    from_string(const char * str);
};

#endif //LARGEINT_H
