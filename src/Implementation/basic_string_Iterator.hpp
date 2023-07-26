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

#include "basic_string_Types.hpp"

#include <iterator>

namespace stds
{
namespace Implementation
{

template<typename T, typename TBasicStringBase>
class base_iterator
{
    public:
        // -- Traits
        using value_type = typename TBasicStringBase::value_type;
        using reference = T&;
        using pointer = T*;
        using difference_type = typename TBasicStringBase::difference_type;
        using iterator_category = std::random_access_iterator_tag;

    private:
        using iterator = base_iterator<value_type, TBasicStringBase>;
        using const_iterator = base_iterator<value_type const, TBasicStringBase>;
        
    public:

        // -- Constructors
        base_iterator(pointer _storage)
        : m_pointer(_storage)
        {
        }
        base_iterator(iterator const& _other)
        : m_pointer(_other.m_pointer)
        {
        }
        base_iterator(const_iterator const& _other)
        : m_pointer(_other.m_pointer)
        {
        }
        
        // -- Assignment
        base_iterator& operator=(base_iterator const& _other)
        {
            m_pointer = _other.m_pointer;
            return *this;
        }

        // -- (+)
        base_iterator operator+(difference_type _offset) { return (base_iterator(*this) += _offset); }
        base_iterator& operator++() { ++m_pointer; return *this; }
        base_iterator& operator+=(difference_type _offset)
        {
            if(_offset >= 0)
            {
                m_pointer += _offset;
            }
            else
            {
                operator-= (-_offset);
            }
            return *this;
        }

        // -- (-)
        base_iterator operator-(difference_type _offset) { return (base_iterator(*this) -= _offset); }
        difference_type operator-(const_iterator const& _other) const { return (m_pointer - _other.m_pointer); }
        base_iterator& operator--() { --m_pointer; return *this; }
        base_iterator& operator-=(difference_type _offset)
        {
            if(_offset >= 0)
            {
                m_pointer -= _offset; 
            }
            else
            {
                operator+= (-_offset);
            }
            return *this;
        }

        // -- Access
        reference operator*() const { return *m_pointer; }
        reference operator[](size_type _pos) { return *(m_pointer + _pos); }
        pointer operator->() const { return m_pointer; }

        // -- Binary operators
        bool operator==(const_iterator const& _other) const { return (m_pointer == _other.m_pointer); }
        bool operator!=(const_iterator const& _other) const { return !(*this == _other); }
        bool operator<(const_iterator const& _other) const { return (m_pointer < _other.m_pointer); }    
        bool operator<=(const_iterator const& _other) const { return !(_other < *this); }
        bool operator>(const_iterator const& _other) const { return (_other < *this); }
        bool operator>=(const_iterator const& _other) const { return !(*this < _other); }

    private:

        pointer m_pointer;

        friend iterator;
        friend const_iterator;
};

} // namespace Implementation
} // namespace stds
