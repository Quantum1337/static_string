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

namespace stds
{
namespace Implementation
{

template<typename T, typename S, typename f>
inline T internal_sto(const S& _str, std::size_t* _pos = nullptr, int _base = 10, f _f = std::strtol)
{
    char* ptr = nullptr;

    T res = static_cast<T>(_f(_str.c_str(), &ptr, _base));

    if(_pos != nullptr)
    {
        const char* const_ptr = ptr;
        *_pos = std::distance(_str.c_str(), const_ptr);
    }

    return res;   
}

template<typename T, typename S, typename f>
inline T internal_sto_fp(const S& _str, std::size_t* _pos = nullptr, f _f = std::strtof)
{
    char* ptr = nullptr;

    T res = static_cast<T>(_f(_str.c_str(), &ptr));

    if(_pos != nullptr)
    {
        const char* const_ptr = ptr;
        *_pos = std::distance(_str.c_str(), const_ptr);
    }

    return res;   
}

template<typename InputIt>
inline unsigned long long internal_hash_fnv_64(InputIt _begin, InputIt _end)
{
    //Fowler-Noll-Vo hash function (non-cryptographic hash)

    static constexpr unsigned long long OFFSET_BASIS = 0xCBF29CE484222325ull;
    static constexpr unsigned long long PRIME        = 0x00000100000001b3ull;

    unsigned long long hash = OFFSET_BASIS;

    while (_begin != _end)
    {
        hash *= PRIME;
        hash ^= *_begin;

        ++_begin;
    }

    return hash;

}

}
}