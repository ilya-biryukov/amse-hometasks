#include <cstring>
#include <iostream>
#include "largeint.h"

LargeInteger::LargeInteger()
    :   value(new int[1]),
        value_size(0),
        value_maxsize(1),
        sign_is_plus(true)
{
    *value = 0;
}

LargeInteger::LargeInteger(const LargeInteger & src)
    :   value(new int[src.value_maxsize]),
        value_size(src.value_size),
        value_maxsize(src.value_maxsize),
        sign_is_plus(src.sign_is_plus)
{
    memcpy(value, src.value, sizeof(*value) * value_maxsize);
}

LargeInteger & LargeInteger::operator =(const LargeInteger & val)
{
    if (this == &val)
        return *this;

    value_size = val.value_size;
    value_maxsize = val.value_maxsize;
    sign_is_plus = val.sign_is_plus;
    delete [] value;

    value = new int[value_maxsize];
    memcpy(value, val.value, sizeof(*value) * value_maxsize);

    return *this;
}

LargeInteger::~LargeInteger()
{
    delete [] value;
}

void LargeInteger::ensure_size(size_t size)
{
    if (value_maxsize >= size)
        return;

    int * new_value = new int[size];
    for (size_t i = 0; i != value_size; ++i)
        new_value[i] = value[i];
    for (size_t i = value_size; i != size; ++i)
        new_value[i] = 0;

    delete[] value;

    value = new_value;
    value_maxsize = size;
}

void LargeInteger::update_size()
{
    for (int * p = value + (value_maxsize - 1); p != value - 1; --p)
    {
        if (*p != 0)
        {
            value_size = p - value + 1;
            return;
        }
    }
    value_size = 0;
}

void LargeInteger::add(const LargeInteger & num)
{
    if (num.sign_is_plus != sign_is_plus)
    {
        sign_is_plus = !sign_is_plus;
        sub(num);
        sign_is_plus = !sign_is_plus;
        return;
    }

    size_t size_needed = (num.value_size > value_size) ? num.value_size + 1 :
                         value_size + 1;
    ensure_size(size_needed);

    int carry   = 0;
    size_t current_digit = 0;

    while (current_digit < num.value_size)
    {
        value[current_digit] += num.value[current_digit] + carry;
        if (value[current_digit] >= LV_BASE)
        {
            value[current_digit] -= LV_BASE;
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
        ++value[current_digit];
        if (value[current_digit] >= LV_BASE)
        {
            value[current_digit] -= LV_BASE;
            carry = 1;
        }
        else
        {
            carry = 0;
        }
        ++current_digit;
    }
    update_size();
}

void LargeInteger::sub(const LargeInteger & num)
{
    if (sign_is_plus != num.sign_is_plus)
    {
        sign_is_plus = !sign_is_plus;
        add(num);
        sign_is_plus = !sign_is_plus;
        return;
    }

    size_t size_needed = (num.value_size > value_size) ? num.value_size
        : value_size;

    ensure_size(size_needed);

    int carry = 0;
    size_t current_digit = 0;

    while (current_digit < num.value_size)
    {
        value[current_digit] -= num.value[current_digit] - carry;
        if (value[current_digit] < 0)
        {
            value[current_digit] += LV_BASE;
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
        --value[current_digit];
        if (value[current_digit] < 0)
        {
            value[current_digit] += LV_BASE;
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
        value[0] = LV_BASE - value[0];
        for (current_digit = 1; current_digit < size_needed; ++current_digit)
            value[current_digit] = LV_BASE - value[current_digit] - 1;
        sign_is_plus = !sign_is_plus;
    }

    update_size();
}

void LargeInteger::mul(const LargeInteger & num)
{
    sign_is_plus =!(sign_is_plus ^ num.sign_is_plus);

    size_t size_needed = num.value_size + value_size;
    if (size_needed == 0)
    {
        return;
    }

    int * new_value = new int[size_needed];
    for (size_t i = 0; i != size_needed; ++i)
    {
        new_value[i] = 0;
    }

    for (size_t i = 0; i != value_size; ++i)
    {
        int carry  = 0;
        int carry2 = 0;
        for (size_t j = 0; j != num.value_size; ++j)
        {
            int tmp = value[i] * num.value[j] + carry;
            carry = tmp / LV_BASE;
            new_value[i + j] += (tmp % LV_BASE) + carry2;

            if (new_value[i + j] >= LV_BASE)
            {
                new_value[i + j] -= LV_BASE;
                carry2 = 1;
            }
            else
            {
                carry2 = 0;
            }
        }

        carry += carry2;
        size_t j = num.value_size;
        while (carry)
        {
            new_value[i + j] += carry;

            carry = new_value[i + j] / LV_BASE;
            new_value[i + j] %= LV_BASE;
        }
    }

    delete[] value;
    value = new_value;
    value_maxsize = size_needed;
    update_size();
}

void LargeInteger::from_string(const std::string & str)
{
    sign_is_plus = (str[0] != '-');
    size_t str_length = str.length();

    size_t digits_required = str_length / LV_DIGITCHARS;
    if (str_length % LV_DIGITCHARS)
        ++digits_required;

    ensure_size(digits_required);

    int digit   = 0;
    int ten_pow = 1;
    size_t chars_read = 0;
    int * value_ptr = value;

    std::string::const_reverse_iterator rit = str.rbegin();
    std::string::const_reverse_iterator rend = str.rend();
    if (!sign_is_plus)
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
        *value_ptr = digit;

    update_size();
}\

std::string LargeInteger::to_string()
{
    if (value_size == 0)
        return "0";

    size_t str_length = value_size * LV_DIGITCHARS + 1;

    if (!sign_is_plus)
        ++str_length;

    char * str = new char[str_length];
    char * str_it = str + str_length - 1;
    *str_it = '\0';
    --str_it;

    for (size_t i = 0; i < value_size; ++i)
    {
        int val = value[i];
        for (size_t j = 0; j < LV_DIGITCHARS; ++j)
        {
            int digit = val % 10;
            val /= 10;

            *str_it = digit + '0';

            --str_it;
        }
    }

    str_it = str;
    if (!sign_is_plus)
        ++str_it;

    while (*str_it == '0')
        ++str_it;

    if (!sign_is_plus)
    {
        --str_it;
        *str_it = '-';
    }

    std::string res_str(str_it);
    delete [] str;

    return res_str;
}
