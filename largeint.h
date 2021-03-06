#ifndef LARGEINT_H
#define LARGEINT_H

#include <string>
#include <cstdint>

class DigitContainer;
class DigitProxy;
class LargeInteger;

class DigitProxy
{
public:
    DigitProxy(DigitContainer & container, size_t index);
    DigitProxy(const DigitProxy & src);

    int32_t operator = (int32_t rhs);
    int32_t operator ++ ();
    int32_t operator ++ (int);
    int32_t operator -- ();
    int32_t operator -- (int);
    operator int32_t() const;
private:
    DigitContainer & m_container;
    int32_t          m_index;
};

class DigitContainer
{
public:
    DigitContainer();
    DigitContainer(const DigitContainer & src);
    ~DigitContainer();

    DigitContainer & operator = (const DigitContainer & val);    
    DigitProxy       operator [](size_t i);
    int32_t          operator [](size_t i) const;

    size_t  get_size() const;
    int32_t get_digit(size_t index) const;
    void    set_digit(size_t index, int32_t value);
    void    optimize();
private:
    void swap(DigitContainer & val);
    void resize_digits(size_t new_size);
    void grow(size_t index_needed);

    int32_t * m_digits;
    size_t    m_digits_size;
    size_t    m_actual_size;
};

class LargeInteger
{
public:
    explicit LargeInteger();
    LargeInteger(const LargeInteger & src);
    LargeInteger(int32_t src);

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
    DigitContainer m_digits;
    bool           m_negative;

    void sdiv(int d, LargeInteger & q, int & r);
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
