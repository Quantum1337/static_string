/*
MIT License

Copyright (c) 2023 Quantum1337 (Christian Knupfer)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

#include "Implementation/basic_string.hpp"
#include "Implementation/basic_string_Impl.hpp"
#include <limits>
#include <charconv>

namespace stds
{

template<Implementation::size_type Size = static_cast<Implementation::size_type>(0)>
using string = stds::Implementation::basic_string<char, std::char_traits<char>, Size>;

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

int stoi(const string<>& _str, std::size_t* _pos = nullptr, int _base = 10)
{
    return Implementation::internal_sto<int>(_str, _pos, _base, std::strtol);
}

long stol(const string<>& _str, std::size_t* _pos = nullptr, int _base = 10)
{
    return Implementation::internal_sto<long>(_str, _pos, _base, std::strtol);
}

long long stoll(const string<>& _str, std::size_t* _pos = nullptr, int _base = 10)
{
    return Implementation::internal_sto<long long>(_str, _pos, _base, std::strtoll);
}

unsigned long stoul(const string<>& _str, std::size_t* _pos = nullptr, int _base = 10)
{
    return Implementation::internal_sto<unsigned long>(_str, _pos, _base, std::strtoul);
}

unsigned long long stoull(const string<>& _str, std::size_t* _pos = nullptr, int _base = 10)
{
    return Implementation::internal_sto<unsigned long long>(_str, _pos, _base, std::strtoull);
}

float stof(const string<>& _str, std::size_t* _pos = nullptr)
{
    return Implementation::internal_sto_fp<float>(_str, _pos, std::strtof);
}

double stod(const string<>& _str, std::size_t* _pos = nullptr)
{
    return Implementation::internal_sto_fp<double>(_str, _pos, std::strtod);
}

long double stold(const string<>& _str, std::size_t* _pos = nullptr)
{
    return Implementation::internal_sto_fp<long double>(_str, _pos, std::strtold);
}

}

namespace std
{

template<>
struct hash<stds::string<>>
{
    unsigned long long operator()(stds::string<> const& _s) const noexcept
    {
        return stds::Implementation::internal_hash_fnv_64(_s.begin(), _s.end());
    }
};

template<stds::string<>::size_type Size>
struct hash<stds::string<Size>>
{
    unsigned long long operator()(stds::string<Size> const& _s) const noexcept
    {
        return stds::Implementation::internal_hash_fnv_64(_s.begin(), _s.end());
    }
};

}
