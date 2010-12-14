#include <cstring>
#include <iostream>
#include "largeint.h"

LargeInteger::LargeInteger()
    :   m_digits(0),
        m_digits_size(0),
        m_digitsarr_size(0),
        m_negative(false)
{
}

LargeInteger::LargeInteger(const LargeInteger & src)
    :   m_digits(new int[src.m_digitsarr_size]),
        m_digits_size(src.m_digits_size),
        m_digitsarr_size(src.m_digitsarr_size),
        m_negative(src.m_negative)
{
    memcpy(m_digits, src.m_digits, sizeof(*m_digits) * m_digitsarr_size);
}

LargeInteger::LargeInteger(int src)
    :   m_digits(new int[LV_INTSIZE]),
        m_digits_size(0),
        m_digitsarr_size(LV_INTSIZE),
        m_negative(src < 0)
{
    if (src < 0)
        src = -src;
    int * it = m_digits;
    int * it_end = m_digits + m_digitsarr_size;
    while (src)
    {
        *it = src % LV_BASE;

        src /= LV_BASE;
        ++it;
    }
    while (*it != *it_end)
    {
        *it = 0;
        ++it;
    }

    update_size();
}

LargeInteger::~LargeInteger()
{
    delete [] m_digits;
}


LargeInteger & LargeInteger::operator =(const LargeInteger & val)
{
    if (this != &val)
        LargeInteger(val).swap(*this);

    return *this;
}

void LargeInteger::swap(LargeInteger & val)
{
    std::swap(m_digits, val.m_digits);
    std::swap(m_digits_size, val.m_digits_size);
    std::swap(m_digitsarr_size, val.m_digitsarr_size);
    std::swap(m_negative, val.m_negative);
}


void LargeInteger::ensure_size(size_t size)
{
    if (m_digitsarr_size >= size)
        return;
    int * new_value = new int[size];
    for (size_t i = 0; i != m_digits_size; ++i)
        new_value[i] = m_digits[i];
    for (size_t i = m_digits_size; i != size; ++i)
        new_value[i] = 0;

    delete[] m_digits;

    m_digits = new_value;
    m_digitsarr_size = size;
}

void LargeInteger::update_size()
{
    for (int * p = m_digits + (m_digitsarr_size - 1); p != m_digits - 1; --p)
    {
        if (*p != 0)
        {
            m_digits_size = p - m_digits + 1;
            return;
        }
    }

    m_digits_size = 0;

    delete[] m_digits;
    m_digits = 0;

    m_digitsarr_size = 0;

    m_negative = false;
}

LargeInteger & LargeInteger::operator += (const LargeInteger & rhs)
{
    if (rhs.m_negative != m_negative)
    {
        m_negative = !m_negative;
        *this -= rhs;
        m_negative = !m_negative;
        return *this;
    }

    size_t size_needed = (rhs.m_digits_size > m_digits_size) ? rhs.m_digits_size + 1 :
                         m_digits_size + 1;
    ensure_size(size_needed);

    int carry   = 0;
    size_t current_digit = 0;

    while (current_digit < rhs.m_digits_size)
    {
        m_digits[current_digit] += rhs.m_digits[current_digit] + carry;
        if (m_digits[current_digit] >= LV_BASE)
        {
            m_digits[current_digit] -= LV_BASE;
            carry = 1;
        }
        else
        {
            carry = 0;
        }

        ++current_digit;
    }
    while (carry != 0)
    {
        ++m_digits[current_digit];
        if (m_digits[current_digit] >= LV_BASE)
        {
            m_digits[current_digit] -= LV_BASE;
            carry = 1;
        }
        else
        {
            carry = 0;
        }
        ++current_digit;
    }
    update_size();

    return *this;
}

LargeInteger & LargeInteger::operator -= (const LargeInteger & rhs)
{
    if (m_negative != rhs.m_negative)
    {
        m_negative = !m_negative;
        *this += rhs;
        m_negative = !m_negative;
        return *this;
    }

    size_t size_needed = (rhs.m_digits_size > m_digits_size) ? rhs.m_digits_size
        : m_digits_size;

    ensure_size(size_needed);

    int carry = 0;
    size_t current_digit = 0;

    while (current_digit < rhs.m_digits_size)
    {
        m_digits[current_digit] -= rhs.m_digits[current_digit] - carry;
        if (m_digits[current_digit] < 0)
        {
            m_digits[current_digit] += LV_BASE;
            carry = -1;
        }
        else
        {
            carry = 0;
        }

        ++current_digit;
    }

    while (carry != 0 && current_digit < size_needed)
    {
        --m_digits[current_digit];
        if (m_digits[current_digit] < 0)
        {
            m_digits[current_digit] += LV_BASE;
            carry = -1;
        }
        else
        {
            carry = 0;
        }

        ++current_digit;
    }

    if (carry)
    {
        m_digits[0] = LV_BASE - m_digits[0];
        for (current_digit = 1; current_digit < size_needed; ++current_digit)
            m_digits[current_digit] = LV_BASE - m_digits[current_digit] - 1;
        m_negative = !m_negative;
    }

    update_size();

    return *this;
}

LargeInteger & LargeInteger::operator *= (const LargeInteger & rhs)
{
    m_negative = (m_negative != rhs.m_negative);

    size_t size_needed = rhs.m_digits_size + m_digits_size;
    if (size_needed == 0)
    {
        return *this;
    }

    int * new_digits = new int[size_needed];
    for (size_t i = 0; i != size_needed; ++i)
    {
        new_digits[i] = 0;
    }

    for (size_t i = 0; i != m_digits_size; ++i)
    {
        int carry  = 0;
        int carry2 = 0;
        for (size_t j = 0; j != rhs.m_digits_size; ++j)
        {
            int tmp = m_digits[i] * rhs.m_digits[j] + carry;
            carry = tmp / LV_BASE;
            new_digits[i + j] += (tmp % LV_BASE) + carry2;

            if (new_digits[i + j] >= LV_BASE)
            {
                new_digits[i + j] -= LV_BASE;
                carry2 = 1;
            }
            else
            {
                carry2 = 0;
            }
        }

        carry += carry2;
        size_t j = rhs.m_digits_size;
        while (carry)
        {
            new_digits[i + j] += carry;

            carry = new_digits[i + j] / LV_BASE;
            new_digits[i + j] %= LV_BASE;
        }
    }

    delete[] m_digits;
    m_digits = new_digits;
    m_digitsarr_size = size_needed;
    update_size();

    return *this;
}

LargeInteger LargeInteger::operator - ()
{
    LargeInteger r(*this);
    r.m_negative = !m_negative;

    return r;
}

void LargeInteger::from_string(const std::string & str)
{
    m_negative = (str[0] == '-');
    size_t str_length = str.length();

    size_t digits_required = str_length / LV_DIGITCHARS;
    if (str_length % LV_DIGITCHARS)
        ++digits_required;

    ensure_size(digits_required);

    int digit   = 0;
    int ten_pow = 1;
    size_t chars_read = 0;
    int * value_ptr = m_digits;

    std::string::const_reverse_iterator rit = str.rbegin();
    std::string::const_reverse_iterator rend = str.rend();
    if (m_negative)
        --rend;

    while (rit != rend)
    {
        digit += ten_pow *((*rit) - '0');
        ten_pow *= 10;

        ++chars_read;
        if (chars_read == LV_DIGITCHARS)
        {
            *value_ptr = digit;

            digit = 0;
            ten_pow = 1;
            chars_read = 0;

            ++value_ptr;
        }

        ++rit;
    }
    if (chars_read)
    {
        *value_ptr = digit;
    }

    update_size();
}

std::string LargeInteger::to_string() const
{
    if (m_digits_size == 0)
        return "0";

    size_t str_length = m_digits_size * LV_DIGITCHARS + 1;

    if (m_negative)
        ++str_length;

    char * str = new char[str_length];
    char * str_it = str + str_length - 1;
    *str_it = '\0';
    --str_it;

    for (size_t i = 0; i < m_digits_size; ++i)
    {
        int val = m_digits[i];
        for (size_t j = 0; j < LV_DIGITCHARS; ++j)
        {
            int digit = val % 10;
            val /= 10;

            *str_it = digit + '0';

            --str_it;
        }
    }

    str_it = str;
    if (m_negative)
        ++str_it;

    while (*str_it == '0')
        ++str_it;

    if (m_negative)
    {
        --str_it;
        *str_it = '-';
    }

    std::string res_str(str_it);
    delete [] str;

    return res_str;
}

LargeInteger operator + (const LargeInteger & lhs, const LargeInteger & rhs)
{
    return LargeInteger(lhs) += rhs;
}

LargeInteger operator - (const LargeInteger & lhs, const LargeInteger & rhs)
{
    return LargeInteger(lhs) -= rhs;
}

LargeInteger operator * (const LargeInteger & lhs, const LargeInteger & rhs)
{
    return LargeInteger(lhs) *= rhs;
}

bool operator == (const LargeInteger & lhs, const LargeInteger & rhs)
{
    if (lhs.m_digits_size != rhs.m_digits_size || lhs.m_negative != rhs.m_negative)
        return false;

    int * p1 = lhs.m_digits;
    int * p2 = rhs.m_digits;
    int * q1 = lhs.m_digits + lhs.m_digits_size;

    while (p1 != q1)
    {
        if (*p1 != *p2)
            return false;

        ++p1;
        ++p2;
    }

    return true;
}

bool operator != (const LargeInteger & lhs, const LargeInteger & rhs)
{
    return !(lhs == rhs);
}

bool operator < (const LargeInteger & lhs, const LargeInteger & rhs)
{
    if (lhs.m_negative && !rhs.m_negative)
        return true;
    if (rhs.m_negative && !lhs.m_negative)
        return false;

    bool abs_less  = false;
    bool abs_equal = true;

    if (lhs.m_digits_size < rhs.m_digits_size)
    {
        abs_less = true;
        abs_equal = false;
    }
    else if (lhs.m_digits_size > rhs.m_digits_size)
    {
        abs_less = false;
        abs_equal = false;
    }
    else
    {
        int * p1 = lhs.m_digits + lhs.m_digits_size - 1;
        int * p2 = rhs.m_digits + rhs.m_digits_size - 1;
        while (p1 != lhs.m_digits - 1)
        {
            if (*p1 != *p2)
            {
                abs_equal = false;
                abs_less = *p1 < *p2;
                break;
            }
            --p1;
            --p2;
        }
    }

    return !abs_equal && ((abs_less && !lhs.m_negative)
                         || (!abs_less && lhs.m_negative));
}

bool operator > (const LargeInteger & lhs, const LargeInteger & rhs)
{
    return rhs < lhs;
}

bool operator >= (const LargeInteger & lhs, const LargeInteger & rhs)
{
    return !(lhs < rhs);
}

bool operator <= (const LargeInteger & lhs, const LargeInteger & rhs)
{
    return !(lhs > rhs);
}

std::ostream & operator << (std::ostream & lhs, const LargeInteger & rhs)
{
    lhs << rhs.to_string();
    return lhs;
}

std::istream & operator >> (std::istream & lhs, LargeInteger & rhs)
{
    std::string str;
    lhs >> str;

    rhs.from_string(str);
    return lhs;
}
