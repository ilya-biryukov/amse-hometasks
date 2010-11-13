#include <cstring>
#include <iostream>
#include "largeint.h"

LargeInteger::LargeInteger()
{
    value_size    = 0;
    value_maxsize = LV_INITSIZE;
    value   = new unsigned int[LV_INITSIZE];
    *value = 0;
}

LargeInteger::~LargeInteger()
{
    delete[] value;
}

void LargeInteger::ensure_size(size_t size)
{
    if (value_maxsize >= size)
        return;

    unsigned int * new_value = new unsigned int[size];
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
    for (unsigned int * p = value + (value_maxsize - 1); p != value - 1; --p)
    {
        if (*p != 0)
        {
            value_size = p - value + 1;
            return;
        }
    }
    value_size = 0;
}

void LargeInteger::print_val()
{
    std::cout << "Number :: \n";
    std::cout << "Max Size -> " << value_maxsize << "\n";
    std::cout << "Size     -> " << value_size << "\n";
    for (size_t i = 0; i < value_maxsize; ++i)
    {
        std::cout << "Digit " << i << " -> " << value[i] << "\n";
    }

    std::cout.flush();
}

void LargeInteger::add(LargeInteger & num)
{
    size_t size_needed = (num.value_size > value_size) ? num.value_size + 1 :
                         value_size + 1;
    ensure_size(size_needed);

    unsigned int carry   = 0;
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

void LargeInteger::from_string(const char * str)
{
    size_t str_length = strlen(str);

    size_t digits_required = str_length / LV_DIGITCHARS;
    if (str_length % LV_DIGITCHARS)
        ++digits_required;

    ensure_size(digits_required);

    unsigned int digit   = 0;
    unsigned int ten_pow = 1;
    size_t chars_read = 0;
    unsigned int * value_ptr = value;

    const char * rstr_begin = str + str_length - 1;
    const char * rstr_end   = str - 1;

    while (rstr_begin != rstr_end)
    {
        digit += ten_pow *((*rstr_begin) - '0');
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

        --rstr_begin;
    }
    if (chars_read)
        *value_ptr = digit;

    update_size();
}

char * LargeInteger::to_string()
{
    if (value_size == 0)
        return "0";

    size_t str_length = value_size * LV_DIGITCHARS + 1;
    char * str = new char[str_length];
    char * str_it = str + str_length - 1;
    *str_it = '\0';
    --str_it;

    for (size_t i = 0; i < value_size; ++i)
    {
        unsigned int val = value[i];
        for (size_t j = 0; j < LV_DIGITCHARS; ++j)
        {
            unsigned int digit = val % 10;
            val /= 10;

            *str_it = digit + '0';

            --str_it;
        }
    }

    str_it = str;

    while (*str_it == '0')
        ++str_it;

    if (str_it != str)
    {
        size_t new_str_length = str_length - (str_it - str);
        char * new_str = new char[new_str_length];
        memcpy(new_str, str_it, sizeof(char) * new_str_length);
        delete[] str;
        str = new_str;
    }

    return str;
}
