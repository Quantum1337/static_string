
#pragma once

#include <functional>
#include "string.hpp"

namespace std
{

template<>
struct hash<stds::string<>>
{
    size_t operator()(stds::string<> const& _s) const noexcept
    {
        static_cast<void>(_s);

        return 0;
    }
};

template<stds::string<>::size_type Size>
struct hash<stds::string<Size>>
{
    size_t operator()(stds::string<Size> const& _s) const noexcept
    {
        static_cast<void>(_s);

        return 0;
    }
};

}