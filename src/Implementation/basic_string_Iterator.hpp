
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
