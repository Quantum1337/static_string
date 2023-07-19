
#pragma once

#include "basic_string.hpp"

namespace stds
{

template<Implementation::size_type Size = static_cast<Implementation::size_type>(0)>
using string = stds::basic_string<char, std::char_traits<char>, Size>;

}
