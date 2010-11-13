#ifndef LARGEINT_H
#define LARGEINT_H

#include <string>

const unsigned int LV_BASE = 1000000000;
const size_t LV_DIGITCHARS = 9;

struct LargeInteger
{
    explicit LargeInteger();
    LargeInteger(const LargeInteger & src);
    ~LargeInteger();

    LargeInteger & operator=(const LargeInteger & val);

    unsigned int *  value;
    size_t          value_size;
    size_t          value_maxsize;

    void    add(LargeInteger & num);
    void    ensure_size(size_t size);
    void    update_size();
    char *  to_string();
    void    from_string(const char * str);
};

#endif //LARGEINT_H
