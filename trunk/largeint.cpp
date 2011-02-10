#include <cstring>
#include <iostream>
#include <cstdint>
#include <limits>
#include <algorithm>
#include "largeint.h"

const int64_t BASE = static_cast<int64_t>(std::numeric_limits<int32_t>::max()) + 1;
const size_t DIGIT_DECIMAL_SIZE = 10;

/* Digit Proxy */

DigitProxy::DigitProxy(DigitContainer & container, size_t index)
    : m_container(container),
      m_index(index)
{
}

DigitProxy::DigitProxy(const DigitProxy & src)
    : m_container(src.m_container),
      m_index(src.m_index)
{
}

int32_t DigitProxy::operator = (int32_t rhs)
{
    m_container.set_digit(m_index, rhs);
    return rhs;
}

DigitProxy::operator int32_t() const
{
    return m_container.get_digit(m_index);
}

int32_t DigitProxy::operator ++()
{
    int32_t v = m_container.get_digit(m_index);
    m_container.set_digit(m_index, ++v);
    return v;
}

int32_t DigitProxy::operator ++(int)
{
    int32_t v = m_container.get_digit(m_index);
    m_container.set_digit(m_index, v + 1);
    return v;
}

int32_t DigitProxy::operator --()
{
    int32_t v = m_container.get_digit(m_index);
    m_container.set_digit(m_index, --v);
    return --v;
}

int32_t DigitProxy::operator --(int)
{
    int32_t v = m_container.get_digit(m_index);
    m_container.set_digit(m_index, v - 1);
    return v;
}

/* DigitContainer */

DigitContainer::DigitContainer()
    : m_digits(0),
      m_digits_size(0)
{
}

void DigitContainer::optimize()
{
    size_t actual_size = get_size();
    if (actual_size < m_digits_size)
    {
        resize_digits(actual_size);
    }
}

DigitContainer::DigitContainer(const DigitContainer & src)
{
    if (src.m_digits_size != 0)
    {
        m_digits = new int32_t[src.m_digits_size];
        memcpy(m_digits, src.m_digits, sizeof(int32_t) * src.m_digits_size);
    }
    else
    {
        m_digits = src.m_digits;
    }

    m_digits_size = src.m_digits_size;
}

DigitContainer::~DigitContainer()
{
    if (m_digits_size != 0)
        delete [] m_digits;
}

int32_t DigitContainer::get_digit(size_t index) const
{
    if (m_digits_size == 0 && index == 0)
    {
        return reinterpret_cast<int32_t>(m_digits);
    }
    else if (index >= m_digits_size)
    {
        return 0;
    }
    else
    {
        return m_digits[index];
    }
}

void DigitContainer::set_digit(size_t index, int32_t value)
{
    if (m_digits_size == 0 && index == 0)
    {
        m_digits = reinterpret_cast<int32_t *>(value);
        return;
    }
    else if (index >= m_digits_size)
    {
        grow(index);
    }

    m_digits[index] = value;
}

void DigitContainer::grow(size_t index_needed)
{
    if (index_needed < m_digits_size)
        return;

    size_t new_size = m_digits_size;

    if (new_size == 0)
        new_size = 1;

    while (new_size <= index_needed)
        new_size *= 2;

    resize_digits(new_size);
}

size_t DigitContainer::get_size() const
{
    if (m_digits_size == 0)
        return (reinterpret_cast<int32_t>(m_digits) == 0) ? 0 : 1;

    size_t size = m_digits_size - 1;
    while (m_digits[size] == 0 && size != 0)
    {
        --size;
    }

    return ++size;
}

void DigitContainer::resize_digits(size_t new_size)
{
    if (new_size == m_digits_size)
        return;

    int32_t * new_digits;
    if (new_size == 0)
    {
        new_digits = reinterpret_cast<int32_t*>(m_digits[0]);
    }
    else
    {
        new_digits = new int32_t[new_size];
    }

    for (size_t i = 0; i < m_digits_size && i < new_size; ++i)
    {
        new_digits[i] = m_digits[i];
    }

    for (size_t i = m_digits_size; i < new_size; ++i)
    {
        new_digits[i] = 0;
    }

    if (m_digits_size == 0)
    {
        new_digits[0] = reinterpret_cast<int32_t>(m_digits);
    }
    else
    {
        delete[] m_digits;
    }

    m_digits = new_digits;
    m_digits_size = new_size;
}

void DigitContainer::swap(DigitContainer & val)
{
    std::swap(m_digits, val.m_digits);
    std::swap(m_digits_size, val.m_digits_size);
}

DigitContainer & DigitContainer::operator = (const DigitContainer & rhs)
{
    if (this != &rhs)
        DigitContainer(rhs).swap(*this);
    return *this;
}

DigitProxy DigitContainer::operator [](size_t i)
{
    return DigitProxy(*this, i);
}

int32_t DigitContainer::operator [](size_t i) const
{
    return get_digit(i);
}

/* LargeInteger */

LargeInteger::LargeInteger()
    : m_negative(false)
{
}

LargeInteger::LargeInteger(const LargeInteger & src)
    :   m_digits(src.m_digits),
        m_negative(src.m_negative)
{
}

LargeInteger::LargeInteger(int32_t src)
    :   m_negative(src < 0)
{
    if (src < 0)
        src = -src;
    m_digits[0] = src;
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
    std::swap(m_negative, val.m_negative);
}


void LargeInteger::sdiv(int32_t d, LargeInteger & q, int32_t & r)
{
    m_negative = (m_negative != (d < 0));
    int32_t rm = 0;
    q = 0;

    for (int i = m_digits.get_size() - 1; i >= 0; --i)
    {
        int64_t tmp = static_cast<int64_t>(rm) * BASE + static_cast<int64_t>(m_digits[i]);
        q.m_digits[i] = static_cast<int32_t>(tmp / d);
        rm = tmp - q.m_digits[i] * d;
    }

    r = rm;
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

    int32_t carry = 0;
    size_t current_digit = 0;
    size_t rhs_size = rhs.m_digits.get_size();
    while (current_digit < rhs_size)
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
        if (++m_digits[current_digit] < 0) // if we got integer overflow
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

    return *this;
}

LargeInteger & LargeInteger::operator -= (const LargeInteger & rhs)
{
    using namespace std;

    if (m_negative != rhs.m_negative)
    {
        m_negative = !m_negative;
        *this += rhs;
        m_negative = !m_negative;
        return *this;
    }

    size_t rhs_size = rhs.m_digits.get_size();
    size_t lhs_size = m_digits.get_size();

    size_t size_needed = max(rhs_size, lhs_size);

    int32_t carry = 0;
    size_t current_digit = 0;

    while (current_digit < rhs.m_digits.get_size())
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
//        int32_t digit = m_digits[current_digit];
//        --digit;
        if (--m_digits[current_digit] < 0)
        {
            m_digits[current_digit] = std::numeric_limits<int32_t>::max();
            carry = -1;
        }
        else
        {
            carry = 0;
        }

//        m_digits[current_digit] = digit;
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
        m_digits.optimize();
    }

    return *this;
}

LargeInteger & LargeInteger::operator *= (const LargeInteger & rhs)
{
    m_negative = (m_negative != rhs.m_negative);

    size_t rhs_size = rhs.m_digits.get_size();
    size_t lhs_size = m_digits.get_size();
    size_t size_needed = rhs_size + lhs_size;
    if (size_needed == 0)
    {
        return *this;
    }

    DigitContainer new_digits;

    for (size_t i = 0; i != lhs_size; ++i)
    {
        int32_t carry  = 0;
        int32_t carry2 = 0;

        for (size_t j = 0; j != rhs_size; ++j)
        {
            int64_t tmp = static_cast<int64_t>(m_digits[i]) * static_cast<int64_t>(rhs.m_digits[j]) + carry;
            carry = tmp / BASE;

            int32_t digit = new_digits[i + j] + (tmp % BASE) + carry2;

            if (digit < 0) // overflow
            {
                digit = 0;
                carry2 = 1;
            }
            else
            {
                carry2 = 0;
            }

            new_digits[i + j] = digit;
        }

        carry += carry2;
        size_t j = rhs_size;
        while (carry)
        {
            int64_t tmp = new_digits[i + j] + carry;

            carry =  tmp / BASE;
            new_digits[i + j] = tmp % BASE;
        }
    }

    m_digits = new_digits;

    if (m_negative && m_digits.get_size() == 0)
        m_negative = false;
    m_digits.optimize();
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
    (*this) = 0;

    int32_t digit = 0;

    for (size_t i = 0; i < str.length(); ++i)
    {
        if (str[i] == '-')
            continue;
        digit = str[i] - '0';
        (*this) *= 10;
        (*this) += digit;
    }

    m_negative = (str[0] == '-');
}

std::string LargeInteger::to_string() const
{
    if (*this == 0)
        return std::string("0");

    size_t str_length = m_digits.get_size() * DIGIT_DECIMAL_SIZE + 2;

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
    size_t lhs_size = lhs.m_digits.get_size();
    size_t rhs_size = rhs.m_digits.get_size();
    if (lhs_size != rhs_size || lhs.m_negative != rhs.m_negative)
        return false;

    for (size_t i = 0; i < lhs_size; ++i)
    {
        if (lhs.m_digits[i] != rhs.m_digits[i])
            return false;
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

    size_t lhs_size = lhs.m_digits.get_size();
    size_t rhs_size = rhs.m_digits.get_size();

    if (lhs_size < rhs_size)
    {
        abs_less = true;
        abs_equal = false;
    }
    else if (lhs_size > rhs_size)
    {
        abs_less = false;
        abs_equal = false;
    }
    else
    {
        for (size_t i = 0; i < lhs_size; ++i)
        {
            if (lhs.m_digits[i] != rhs.m_digits[i])
            {
                abs_equal = false;
                abs_less = lhs.m_digits[i] < rhs.m_digits[i];
                break;
            }
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
