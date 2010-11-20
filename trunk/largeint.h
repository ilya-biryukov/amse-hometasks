#ifndef LARGEINT_H
#define LARGEINT_H

#include <string>

const int LV_BASE = 10000;
const size_t LV_DIGITCHARS = 4;

class LargeInteger
{
public:
    explicit LargeInteger();
    LargeInteger(const LargeInteger & src);
    ~LargeInteger();

    LargeInteger & operator=(const LargeInteger & val);

    void        add(const LargeInteger & num);
    void        sub(const LargeInteger & num);
    void        mul(const LargeInteger & num);
    std::string to_string();
    void        from_string(const std::string & str);
private:
    int *  value;
    size_t          value_size;
    size_t          value_maxsize;
    bool            sign_is_plus;

    void    ensure_size(size_t size);
    void    update_size();
};

#endif //LARGEINT_H
