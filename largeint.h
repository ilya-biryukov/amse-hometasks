#ifndef LARGEINT_H
#define LARGEINT_H

#include <string>
#include <cstdint>

class LargeInteger
{
public:
    explicit LargeInteger();
    LargeInteger(const LargeInteger & src);
    LargeInteger(int32_t src);
    ~LargeInteger();

    LargeInteger & operator = (const LargeInteger & val);

    LargeInteger & operator += (const LargeInteger & rhs);
    LargeInteger & operator -= (const LargeInteger & rhs);
    LargeInteger & operator *= (const LargeInteger & rhs);
    LargeInteger & operator /= (int32_t rhs);

    LargeInteger operator - ();

    std::string to_string() const;
    void        from_string(const std::string & str);
    void        swap(LargeInteger & val);

    friend bool operator == (const LargeInteger & lhs, const LargeInteger & rhs);
    friend bool operator < (const LargeInteger & lhs, const LargeInteger & rhs);
private:
    int32_t *   m_digits;
    size_t      m_digits_size;
    size_t      m_digitsarr_size;
    bool        m_negative;

    void    ensure_size(size_t size);
    void    update_size();
    void    sdiv(int d, LargeInteger & q, int & r);
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
