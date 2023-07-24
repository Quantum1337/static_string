#pragma once

namespace stds
{
namespace Implementation
{

template<typename T, typename S, typename f>
T internal_sto(const S& _str, std::size_t* _pos = nullptr, int _base = 10, f _f = std::strtol)
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
T internal_sto_fp(const S& _str, std::size_t* _pos = nullptr, f _f = std::strtof)
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

}
}