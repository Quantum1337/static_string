
#pragma once

#include "basic_string.hpp"
#include <limits>
#include <charconv>

namespace stds
{

template<Implementation::size_type Size = static_cast<Implementation::size_type>(0)>
using string = stds::basic_string<char, std::char_traits<char>, Size>;

template<typename T, size_t Size = std::numeric_limits<T>::digits10 + 2>
string<Size> to_string(T _value)
{
    char buf[Size];

    std::to_chars_result res = std::to_chars(buf, &buf[Size], _value);
    BASIC_STRING_ASSERT(std::errc() == res.ec);

    return string<Size>(buf, res.ptr);
}

template<typename T, size_t Size = std::numeric_limits<T>::digits10 + 2>
string<> to_string(T _value, string<>::value_type* _buf, size_t _size = Size)
{
    std::to_chars_result res = std::to_chars(_buf, &_buf[_size], _value);
    BASIC_STRING_ASSERT(std::errc() == res.ec);

    return string<>(_buf, res.ptr, _size);
}

}
