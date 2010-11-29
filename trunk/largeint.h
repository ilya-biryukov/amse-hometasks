#ifndef LARGEINT_H
#define LARGEINT_H

#include <string>

const int LV_BASE = 10000;
const size_t LV_DIGITCHARS = 4;
const size_t LV_INTSIZE = 3;

class LargeInteger
{
public:
    explicit LargeInteger();
    LargeInteger(const LargeInteger & src);
    LargeInteger(int src);
    ~LargeInteger();

    LargeInteger & operator = (const LargeInteger & val);

    LargeInteger & operator += (const LargeInteger & rhs);
    LargeInteger & operator -= (const LargeInteger & rhs);
    LargeInteger & operator *= (const LargeInteger & rhs);

    LargeInteger operator - ();

    std::string to_string() const;
    void        from_string(const std::string & str);
    void        swap(LargeInteger & val);

    friend bool operator == (const LargeInteger & lhs, const LargeInteger & rhs);
    friend bool operator < (const LargeInteger & lhs, const LargeInteger & rhs);
private:
    int *   m_digits;
    size_t  m_digits_size;
    size_t  m_digitsarr_size;
    bool    m_negative;

    void    ensure_size(size_t size);
    void    update_size();
};


LargeInteger  operator + (const LargeInteger & lhs, const LargeInteger & rhs);
LargeInteger  operator - (const LargeInteger & lhs, const LargeInteger & rhs);
LargeInteger  operator * (const LargeInteger & lhs, const LargeInteger & rhs);

bool operator == (const LargeInteger & lhs, const LargeInteger & rhs);
bool operator != (const LargeInteger & lhs, const LargeInteger & rhs);

bool operator < (const LargeInteger & lhs, const LargeInteger & rhs);
bool operator > (const LargeInteger & lhs, const LargeInteger & rhs);
bool operator >= (const LargeInteger & lhs, const LargeInteger & rhs);
bool operator <= (const LargeInteger & lhs, const LargeInteger & rhs);

std::ostream & operator << (std::ostream & lhs, const LargeInteger & rhs);
std::istream & operator >> (std::istream & lhs, LargeInteger & rhs);

#endif //LARGEINT_H
