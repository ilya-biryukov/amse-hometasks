#include <cstring>
#include <iostream>
#include <cstdint>
#include <cstdio>
#include <limits>
#include <algorithm>
#include "largeint.h"

const int64_t BASE = static_cast<int64_t>(std::numeric_limits<int32_t>::max()) + 1;
const size_t DIGIT_DECIMAL_SIZE = 10;

LargeInteger::LargeInteger()
    :   m_digits(0),
        m_digits_size(0),
        m_digitsarr_size(0),
        m_negative(false),
        m_heap_allocated(false)
{
}

LargeInteger::LargeInteger(const LargeInteger & src)
    :   m_digits(0),
        m_digits_size(src.m_digits_size),
        m_digitsarr_size(src.m_digitsarr_size),
        m_negative(src.m_negative),
        m_heap_allocated(src.m_heap_allocated)
{
    if (m_heap_allocated)
    {
        m_digits = new int32_t[m_digitsarr_size];
        memcpy(m_digits, src.m_digits, sizeof(*m_digits) * m_digitsarr_size);
    }
    else
    {
        m_digits = src.m_digits;
    }
}

LargeInteger::LargeInteger(int32_t src)
    :   m_digits(reinterpret_cast<int32_t *>(src)),
        m_digits_size(0),
        m_digitsarr_size(0),
        m_negative(src < 0),
        m_heap_allocated(false)
{
}

LargeInteger::~LargeInteger()
{
    if (m_heap_allocated)
        delete [] m_digits;
}


LargeInteger & LargeInteger::operator = (const LargeInteger & val)
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
    if (!m_heap_allocated)
        allocate_on_heap(reinterpret_cast<int32_t>(m_digits));
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

void LargeInteger::update_size() const
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

void LargeInteger::sdiv(int32_t d, LargeInteger & q, int32_t & r)
{
    m_negative = (m_negative != (d < 0));
    int32_t rm = 0;

    if (m_heap_allocated)
    {
        q = 0;
        q.ensure_size(m_digits_size);

        for (int i = m_digits_size - 1; i >= 0; --i)
        {
            int64_t tmp = static_cast<int64_t>(rm) * BASE + static_cast<int64_t>(m_digits[i]);
            q.m_digits[i] = static_cast<int32_t>(tmp / d);
            rm = tmp - q.m_digits[i] * d;
        }

        r = rm;

        q.update_size();
    }
    else
    {
        q = reinterpret_cast<int32_t>(m_digits) / d;
        rm = reinterpret_cast<int32_t>(m_digits) % d;
    }
}

LargeInteger & LargeInteger::operator += (const LargeInteger & rhs)
{
    if (!m_heap_allocated && !rhs.m_heap_allocated)
    {
        int64_t res;
        res = static_cast<int64_t>(reinterpret_cast<int32_t>(m_digits)) +
              static_cast<int64_t>(reinterpret_cast<int32_t>(rhs.m_digits));
        if (res >= BASE)
            allocate_on_heap(res);
        else
        {
            m_digits = reinterpret_cast<int32_t *>(static_cast<int32_t>(res));
            m_negative = res < 0;
        }
        return *this;
    }


    if (!m_heap_allocated)
        allocate_on_heap(static_cast<int64_t>(reinterpret_cast<int32_t>(m_digits)));
    if (!rhs.m_heap_allocated)
        rhs.allocate_on_heap(static_cast<int64_t>(reinterpret_cast<int32_t>(rhs.m_digits)));

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

    int32_t carry = 0;
    size_t current_digit = 0;

    while (current_digit < rhs.m_digits_size)
    {
        int64_t tmp = static_cast<int64_t>(m_digits[current_digit]) +
                      static_cast<int64_t>(rhs.m_digits[current_digit]) +
                      carry;
        if (tmp >= BASE)
        {
            tmp -= BASE;
            carry = 1;
        }
        else
        {
            carry = 0;
        }

        m_digits[current_digit] = static_cast<int32_t>(tmp);
        ++current_digit;
    }
    while (carry != 0)
    {
        ++m_digits[current_digit];
        if (m_digits[current_digit] < 0) // if we got integer overflow
        {
            m_digits[current_digit] = 0;
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
    if (!m_heap_allocated && !rhs.m_heap_allocated)
    {
        int64_t res;
        res = static_cast<int64_t>(reinterpret_cast<int32_t>(m_digits)) -
              static_cast<int64_t>(reinterpret_cast<int32_t>(rhs.m_digits));
        if (res < std::numeric_limits<int32_t>::min())
            allocate_on_heap(res);
        else
        {
            m_digits = reinterpret_cast<int32_t*>(static_cast<int32_t>(res));
            m_negative = res < 0;
        }
        return *this;
    }


    if (!m_heap_allocated)
        allocate_on_heap(static_cast<int64_t>(reinterpret_cast<int32_t>(m_digits)));
    if (!rhs.m_heap_allocated)
        rhs.allocate_on_heap(static_cast<int64_t>(reinterpret_cast<int32_t>(rhs.m_digits)));

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

    int32_t carry = 0;
    size_t current_digit = 0;

    while (current_digit < rhs.m_digits_size)
    {
        int64_t tmp = static_cast<int64_t>(m_digits[current_digit]) -
                      static_cast<int64_t>(rhs.m_digits[current_digit]) +
                      carry;

        if (tmp < 0)
        {
            tmp += BASE;
            carry = -1;
        }
        else
        {
            carry = 0;
        }

        m_digits[current_digit] = static_cast<int32_t>(tmp);

        ++current_digit;
    }

    while (carry != 0 && current_digit < size_needed)
    {
        --m_digits[current_digit];
        if (m_digits[current_digit] < 0)
        {
            m_digits[current_digit] = std::numeric_limits<int32_t>::max();
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
        int64_t tmp = BASE - static_cast<int64_t>(m_digits[0]);
        m_digits[0] = static_cast<int32_t>(tmp);
        for (current_digit = 1; current_digit < size_needed; ++current_digit)
        {
            int64_t tmp = BASE -
                          static_cast<int64_t>(m_digits[current_digit]) -
                          1;
            m_digits[current_digit] = static_cast<int32_t>(tmp);
        }
        m_negative = !m_negative;
    }

    update_size();

    return *this;
}

LargeInteger & LargeInteger::operator *= (const LargeInteger & rhs)
{
    if (!m_heap_allocated && !rhs.m_heap_allocated)
    {
        int64_t res;
        res = static_cast<int64_t>(reinterpret_cast<int32_t>(m_digits)) *
              static_cast<int64_t>(reinterpret_cast<int32_t>(rhs.m_digits));
        if (res >= BASE || res < std::numeric_limits<int32_t>::min())
            allocate_on_heap(res);
        else
        {
            m_digits = reinterpret_cast<int32_t *>(static_cast<int32_t>(res));
            m_negative = res < 0;
        }
        return *this;
    }

    if (!m_heap_allocated)
        allocate_on_heap(static_cast<int64_t>(reinterpret_cast<int32_t>(m_digits)));
    if (!rhs.m_heap_allocated)
        rhs.allocate_on_heap(static_cast<int64_t>(reinterpret_cast<int32_t>(rhs.m_digits)));

    m_negative = (m_negative != rhs.m_negative);

    size_t size_needed = rhs.m_digits_size + m_digits_size;
    if (size_needed == 0)
    {
        return *this;
    }

    int32_t * new_digits = new int[size_needed];
    for (size_t i = 0; i != size_needed; ++i)
    {
        new_digits[i] = 0;
    }

    for (size_t i = 0; i != m_digits_size; ++i)
    {
        int32_t carry  = 0;
        int32_t carry2 = 0;
        for (size_t j = 0; j != rhs.m_digits_size; ++j)
        {
            int64_t tmp = static_cast<int64_t>(m_digits[i]) * static_cast<int64_t>(rhs.m_digits[j]) + carry;
            carry = tmp / BASE;
            new_digits[i + j] += (tmp % BASE) + carry2;

            if (new_digits[i + j] < 0) // overflow
            {
                new_digits[i + j] = 0;
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
            int64_t tmp = new_digits[i + j] + carry;

            carry =  tmp / BASE;
            new_digits[i + j] = tmp % BASE;
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
    if (!m_heap_allocated)
        m_digits = reinterpret_cast<int32_t *>(- reinterpret_cast<int32_t>(m_digits));

    return r;
}

void LargeInteger::from_string(const std::string & str)
{
    size_t str_length = str.length();
    (*this) = 0;

    size_t digits_required = str_length / (DIGIT_DECIMAL_SIZE - 1) + 1;
    ensure_size(digits_required);

    int32_t   digit   = 0;

    for (size_t i = 0; i < str.length(); ++i)
    {
        if (str[i] == '-')
            continue;
        digit = str[i] - '0';
        (*this) *= 10;
        (*this) += digit;
    }

    m_negative = (str[0] == '-');
    update_size();
}

std::string LargeInteger::to_string() const
{
    if (!m_heap_allocated)
    {
        char * str = new char[12];
        sprintf(str, "%d", reinterpret_cast<int32_t>(m_digits));
        std::string tmp(str);
        delete[] str;
        return tmp;
    }

    if (m_digits_size == 0)
        return std::string("0");

    size_t str_length = m_digits_size * DIGIT_DECIMAL_SIZE + 2;

    if (m_negative)
        ++str_length;

    char * str = new char[str_length];
    char * str_it = str + str_length - 1;
    *str_it = '\0';
    --str_it;

    LargeInteger tmp(*this);
    while (tmp != 0)
    {
        LargeInteger q;
        int32_t digit;

        tmp.sdiv(10, q, digit);
        tmp = q;
        *str_it = digit + '0';
        --str_it;
    }
    if (m_negative)
    {
        *str_it = '-';
        --str_it;
    }

    std::string res_str(++str_it);
    delete [] str;

    return res_str;
}

void LargeInteger::allocate_on_heap(int64_t value) const
{
    if (m_heap_allocated)
        return;

    m_heap_allocated = true;
    m_digits = new int32_t[2];
    m_digits_size = 2;
    m_digitsarr_size = 2;
    m_negative = value < 0;
    m_digits[0] = value % BASE;
    m_digits[1] = (value / BASE) % BASE;
    if (m_digits[0] < 0)
        m_digits[0] = -m_digits[0];
    if (m_digits[1] < 0)
        m_digits[1] = -m_digits[1];
    update_size();
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
    if (!lhs.m_heap_allocated && !rhs.m_heap_allocated)
    {
        return lhs.m_digits == rhs.m_digits;
    }
    if (!lhs.m_heap_allocated)
        lhs.allocate_on_heap(static_cast<int64_t>(reinterpret_cast<int32_t>(lhs.m_digits)));
    if (!rhs.m_heap_allocated)
        rhs.allocate_on_heap(static_cast<int64_t>(reinterpret_cast<int32_t>(rhs.m_digits)));

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
    if (!lhs.m_heap_allocated && !rhs.m_heap_allocated)
    {
        return reinterpret_cast<int32_t>(lhs.m_digits) < reinterpret_cast<int32_t>(rhs.m_digits);
    }
    if (!lhs.m_heap_allocated)
        lhs.allocate_on_heap(static_cast<int64_t>(reinterpret_cast<int32_t>(lhs.m_digits)));
    if (!rhs.m_heap_allocated)
        rhs.allocate_on_heap(static_cast<int64_t>(reinterpret_cast<int32_t>(rhs.m_digits)));

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

