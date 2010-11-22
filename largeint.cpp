#include <cstring>
#include <iostream>
#include "largeint.h"

LargeInteger::LargeInteger()
    :   digits(new int[1]),
        digits_size(0),
        digitsarr_size(1),
        negative(false)
{
    *digits = 0;
}

LargeInteger::LargeInteger(const LargeInteger & src)
    :   digits(new int[src.digitsarr_size]),
        digits_size(src.digits_size),
        digitsarr_size(src.digitsarr_size),
        negative(src.negative)
{
    memcpy(digits, src.digits, sizeof(*digits) * digitsarr_size);
}

LargeInteger::LargeInteger(int src)
    :   digits(new int[LV_INTSIZE]),
        digits_size(0),
        digitsarr_size(LV_INTSIZE),
        negative(src < 0)
{
    if (src < 0)
        src = -src;
    int * it = digits;
    int * it_end = digits + digitsarr_size;
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
    delete [] digits;
}


LargeInteger & LargeInteger::operator =(const LargeInteger & val)
{
    if (this != &val)
        LargeInteger(val).swap(*this);

    return *this;
}

void LargeInteger::swap(LargeInteger & val)
{
    std::swap(digits, val.digits);
    std::swap(digits_size, val.digits_size);
    std::swap(digitsarr_size, val.digitsarr_size);
    std::swap(negative, val.negative);
}


void LargeInteger::ensure_size(size_t size)
{
    if (digitsarr_size >= size)
        return;

    int * new_value = new int[size];
    for (size_t i = 0; i != digits_size; ++i)
        new_value[i] = digits[i];
    for (size_t i = digits_size; i != size; ++i)
        new_value[i] = 0;

    delete[] digits;

    digits = new_value;
    digitsarr_size = size;
}

void LargeInteger::update_size()
{
    for (int * p = digits + (digitsarr_size - 1); p != digits - 1; --p)
    {
        if (*p != 0)
        {
            digits_size = p - digits + 1;
            return;
        }
    }


    digits_size = 0;
    negative = false;
}

LargeInteger & LargeInteger::operator += (const LargeInteger & rhs)
{
    if (rhs.negative != negative)
    {
        negative = !negative;
        *this -= rhs;
        negative = !negative;
        return *this;
    }

    size_t size_needed = (rhs.digits_size > digits_size) ? rhs.digits_size + 1 :
                         digits_size + 1;
    ensure_size(size_needed);

    int carry   = 0;
    size_t current_digit = 0;

    while (current_digit < rhs.digits_size)
    {
        digits[current_digit] += rhs.digits[current_digit] + carry;
        if (digits[current_digit] >= LV_BASE)
        {
            digits[current_digit] -= LV_BASE;
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
        ++digits[current_digit];
        if (digits[current_digit] >= LV_BASE)
        {
            digits[current_digit] -= LV_BASE;
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
    if (negative != rhs.negative)
    {
        negative = !negative;
        *this += rhs;
        negative = !negative;
        return *this;
    }

    size_t size_needed = (rhs.digits_size > digits_size) ? rhs.digits_size
        : digits_size;

    ensure_size(size_needed);

    int carry = 0;
    size_t current_digit = 0;

    while (current_digit < rhs.digits_size)
    {
        digits[current_digit] -= rhs.digits[current_digit] - carry;
        if (digits[current_digit] < 0)
        {
            digits[current_digit] += LV_BASE;
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
        --digits[current_digit];
        if (digits[current_digit] < 0)
        {
            digits[current_digit] += LV_BASE;
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
        digits[0] = LV_BASE - digits[0];
        for (current_digit = 1; current_digit < size_needed; ++current_digit)
            digits[current_digit] = LV_BASE - digits[current_digit] - 1;
        negative = !negative;
    }

    update_size();

    return *this;
}

LargeInteger & LargeInteger::operator *= (const LargeInteger & rhs)
{
    negative = (negative != rhs.negative);

    size_t size_needed = rhs.digits_size + digits_size;
    if (size_needed == 0)
    {
        return *this;
    }

    int * new_digits = new int[size_needed];
    for (size_t i = 0; i != size_needed; ++i)
    {
        new_digits[i] = 0;
    }

    for (size_t i = 0; i != digits_size; ++i)
    {
        int carry  = 0;
        int carry2 = 0;
        for (size_t j = 0; j != rhs.digits_size; ++j)
        {
            int tmp = digits[i] * rhs.digits[j] + carry;
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
        size_t j = rhs.digits_size;
        while (carry)
        {
            new_digits[i + j] += carry;

            carry = new_digits[i + j] / LV_BASE;
            new_digits[i + j] %= LV_BASE;
        }
    }

    delete[] digits;
    digits = new_digits;
    digitsarr_size = size_needed;
    update_size();

    return *this;
}

LargeInteger LargeInteger::operator - ()
{
    LargeInteger r(*this);
    r.negative = !negative;

    return r;
}

void LargeInteger::from_string(const std::string & str)
{
    negative = (str[0] == '-');
    size_t str_length = str.length();

    size_t digits_required = str_length / LV_DIGITCHARS;
    if (str_length % LV_DIGITCHARS)
        ++digits_required;

    ensure_size(digits_required);

    int digit   = 0;
    int ten_pow = 1;
    size_t chars_read = 0;
    int * value_ptr = digits;

    std::string::const_reverse_iterator rit = str.rbegin();
    std::string::const_reverse_iterator rend = str.rend();
    if (negative)
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
    if (digits_size == 0)
        return "0";

    size_t str_length = digits_size * LV_DIGITCHARS + 1;

    if (negative)
        ++str_length;

    char * str = new char[str_length];
    char * str_it = str + str_length - 1;
    *str_it = '\0';
    --str_it;

    for (size_t i = 0; i < digits_size; ++i)
    {
        int val = digits[i];
        for (size_t j = 0; j < LV_DIGITCHARS; ++j)
        {
            int digit = val % 10;
            val /= 10;

            *str_it = digit + '0';

            --str_it;
        }
    }

    str_it = str;
    if (negative)
        ++str_it;

    while (*str_it == '0')
        ++str_it;

    if (negative)
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
    if (lhs.digits_size != rhs.digits_size || lhs.negative != rhs.negative)
        return false;

    int * p1 = lhs.digits;
    int * p2 = rhs.digits;
    int * q1 = lhs.digits + lhs.digits_size;

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
    if (lhs.negative && !rhs.negative)
        return true;
    if (rhs.negative && !lhs.negative)
        return false;

    bool abs_less  = false;
    bool abs_equal = true;

    if (lhs.digits_size < rhs.digits_size)
    {
        abs_less = true;
        abs_equal = false;
    }
    else if (lhs.digits_size > rhs.digits_size)
    {
        abs_less = false;
        abs_equal = false;
    }
    else
    {
        int * p1 = lhs.digits + lhs.digits_size - 1;
        int * p2 = rhs.digits + rhs.digits_size - 1;
        while (p1 != lhs.digits - 1)
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

    return !abs_equal && ((abs_less && !lhs.negative)
                         || (!abs_less && lhs.negative));
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
