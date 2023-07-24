#pragma once

#include "Implementation/basic_string_Types.hpp"
#include "Implementation/basic_string_Iterator.hpp"
#include "Implementation/basic_string_Assert.hpp"
#include <array>
#include <algorithm>

namespace stds
{

template<typename CharT, typename Traits, Implementation::size_type=static_cast<Implementation::size_type>(0)>
class basic_string;

template<typename CharT, typename Traits>
class basic_string<CharT, Traits>
{
    public:
        // -- Traits
        using traits_type = Traits;
        using value_type = CharT;
        using size_type = typename Implementation::size_type;
        using difference_type = typename Implementation::difference_type;
        using reference = value_type&;
        using const_reference = value_type const&;
        using pointer = value_type*;
        using const_pointer = value_type const*;
        using iterator = Implementation::base_iterator<value_type, basic_string>;
        using const_iterator = Implementation::base_iterator<value_type const, basic_string>;
        using reverse_iterator = std::reverse_iterator<Implementation::base_iterator<value_type, basic_string>>;
        using const_reverse_iterator = std::reverse_iterator<Implementation::base_iterator<value_type const, basic_string>>;

        static const size_type npos = -1;

        // -- Assignment operator
        basic_string& operator=(const basic_string& _other)
        {
            return assign(_other);
        }

        basic_string& operator=(basic_string&& _other)
        {
            return assign(std::move(_other));
        }

        basic_string& operator=(const CharT* _s)
        {
            return assign(_s);
        }

        basic_string& operator=(CharT _ch)
        {
            return assign(1u, _ch);
        }

        basic_string& operator=(std::initializer_list<CharT> _ilist)
        {
            return assign(_ilist);
        }

        // -- Assign
        basic_string& assign(size_type _count, CharT _ch)
        {
            clear();
            this->assert_count_in_range(_count);
            this->unchecked_push_back_count(_count, _ch);  

            return *this;
        }

        basic_string& assign(const basic_string& _other)
        {
            clear();
            internal_rangeInit(_other.begin(), _other.end(), this->back_inserter(*this));

            return *this;
        }

        basic_string& assign(const basic_string& _other,
                             size_type _pos, size_type _count /* =npos ??*/)
        {
            clear();
            internal_rangeInit(_other.begin() + _pos, _other.begin() + (_count + 1u), this->back_inserter(*this));

            return *this;
        }
        
        basic_string& assign(basic_string&& _other)
        { 
            clear();
            internal_rangeInit(std::make_move_iterator(_other.begin()), 
                               std::make_move_iterator(_other.end()), 
                               back_inserter(*this));
            _other.clear();
            return *this;
        }
        
        basic_string& assign(const CharT* _s, size_type _count)
        {
            clear();
            internal_rangeInit(&_s[0], &_s[_count], this->back_inserter(*this));

            return *this;
        }
        
        basic_string& assign(const CharT* _s)
        {
            clear();

            size_type len = this->internal_strlen(_s);
            internal_rangeInit(&_s[0], &_s[len], this->back_inserter(*this));

            return *this;
        }
        
        template<class InputIt>
        basic_string& assign(InputIt _first, InputIt _last)
        {
            clear();
            internal_rangeInit(_first, _last, this->back_inserter(*this));

            return *this;
        }
        
        basic_string& assign(std::initializer_list<CharT> _ilist)
        {
            clear();
            internal_rangeInit(_ilist.begin(), _ilist.end(), this->back_inserter(*this));

            return *this;
        }
        
        // -- Element access
        reference at(size_type _pos)
        {
            assert_access_in_range(_pos);

            return (*this)[_pos];
        }    
        const_reference at(size_type _pos) const
        {
            assert_access_in_range(_pos);

            return (*this)[_pos];
        }

        reference operator[](size_type _pos) { return *(begin() += _pos); }
        const_reference operator[](size_type _pos) const { return *(cbegin() += _pos); }

        reference front() { return *begin(); }
        const_reference front() const { return *cbegin(); }

        reference back() { return *std::prev(end()); }
        const_reference back() const { return *std::prev(cend()); }

        const_pointer c_str() const noexcept { *m_pos = '\0'; return &(*m_begin); }

        const_pointer data() const noexcept { c_str(); }

        // -- Iterators
        iterator begin() noexcept { return m_begin; };
        const_iterator begin() const noexcept { return m_begin; };
        const_iterator cbegin() const noexcept { return m_begin; };

        iterator end() noexcept { return m_pos; };
        const_iterator end() const noexcept { return m_pos; };
        const_iterator cend() const noexcept { return m_pos; };

        reverse_iterator rbegin() noexcept { return reverse_iterator(m_pos); };
        const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(m_pos); };
        const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(m_pos); };

        reverse_iterator rend() noexcept { return reverse_iterator(m_begin); };
        const_reverse_iterator rend() const noexcept { return const_reverse_iterator(m_begin); };
        const_reverse_iterator crend() const noexcept { return const_reverse_iterator(m_begin); };

        // -- Capacity
        bool empty() const { return (m_begin == m_pos); };
        size_type size() const { return std::distance(m_begin, m_pos); };
        size_type length() const { return std::distance(m_begin, m_pos); };
        size_type max_size() const { return m_capacity; };
        void reserve(size_type _new_cap = 0) { assert_count_in_range(_new_cap); };
        size_type capacity() const noexcept { return m_capacity; }
        void shrink_to_fit() { /* It is a non-binding request to reduce capacity() to size(). Doing nothing */ };

        // -- Operations
        void clear() noexcept
        {
            m_pos = m_begin;
        }

        basic_string& insert(size_type _index, size_type _count, CharT _ch)
        {
            assert_space_left();
            assert_access_in_range(_index);

            unchecked_insert_value(begin() += _index, _count, _ch);

            return *this;
        }
        basic_string& insert(size_type _index, const CharT* _s)
        {
            assert_space_left();
            assert_access_in_range(_index);

            size_type len = internal_strlen(_s);
            unchecked_insert_it(begin() += _index, &_s[0], &_s[len]);

            return *this;            
        }
        basic_string& insert( size_type _index, const CharT* _s, size_type _count)
        {
            assert_space_left();
            assert_access_in_range(_index);

            unchecked_insert_it(begin() += _index, &_s[0], &_s[_count]);

            return *this;         
        }
        basic_string& insert(size_type _index, const basic_string& _str)
        {
            assert_space_left();
            assert_access_in_range(_index);

            unchecked_insert_it(begin() += _index, _str.begin(), _str.end());

            return *this; 
        }
        basic_string& insert(size_type _index, const basic_string& _str,
                             size_type _s_index, size_type _count /*= npos */)
        {
            iterator substrStart = to_iterator(_str.begin()) + _s_index;
            iterator substrStop = to_iterator(_str.begin()) + _count;

            assert_space_left();
            assert_access_in_range(_index);
            assert_valid_iterator_pair(substrStart, substrStop);
            BASIC_STRING_ASSERT(static_cast<size_type>(std::distance(substrStart, substrStop)) <= _str.size());

            unchecked_insert_it(begin() += _index, substrStart, substrStop);

            return *this;
        }
        iterator insert(const_iterator _pos, CharT _ch)
        {
            assert_space_left();
            assert_iterator_in_range(_pos);

            return unchecked_insert_value(_pos, 1u, _ch);
        }
        iterator insert(const_iterator _pos, size_type _count, CharT _ch)
        { 
            assert_space_left();
            assert_iterator_in_range(_pos);

            return unchecked_insert_value(_pos, _count, _ch);
        }
        template< class InputIt >
        iterator insert(const_iterator _pos, InputIt _first, InputIt _last)
        {
            assert_space_left();
            assert_iterator_in_range(_pos);
            assert_valid_iterator_pair(_first, _last);

            return unchecked_insert_it(_pos, _first, _last);
        }
        iterator insert(const_iterator _pos, std::initializer_list<CharT> _ilist)
        {
            assert_space_left();
            assert_iterator_in_range(_pos);

            return unchecked_insert_it(_pos, _ilist.begin(), _ilist.end());
        }

        basic_string& erase(size_type _index /*= 0*/, size_type _count /*= npos*/)
        {
            assert_access_in_range(_index);

            unchecked_erase(begin() + _index, std::next(std::min(_count, size() - _index)));

            return *this;
        }
        iterator erase(const_iterator _pos)
        {
            assert_iterator_in_range(_pos);

            return unchecked_erase(_pos, std::next(_pos));
        }
        iterator erase(const_iterator _first, const_iterator _last)
        {
            assert_iterator_pair_in_range(_first, _last);

            return unchecked_erase(_first, _last);
        }

        void push_back(CharT _ch)
        {
            assert_space_left();

            unchecked_push_back(_ch);
        }

        void pop_back()
        {
            assert_has_elements();

            unchecked_pop_back();
        }

        basic_string& append(size_type _count, CharT _ch)
        {
            assert_count_in_range(size() + _count);
            unchecked_push_back_count(_count, _ch);
            return *this;
        }
        basic_string& append(const basic_string& _str)
        {
            assert_count_in_range(size() + std::distance(_str.begin(), _str.end()));
            internal_rangeInit(_str.begin(), _str.end(), back_inserter(*this));
            return *this;
        }
        basic_string& append(const basic_string& _str,
                             size_type _pos, size_type _count /*= npos*/)
        {
            iterator substrStart = to_iterator(_str.begin()) + _pos;
            iterator substrStop = to_iterator(_str.begin()) + (_pos + _count);

            assert_valid_iterator_pair(substrStart, substrStop);
            BASIC_STRING_ASSERT(static_cast<size_type>(std::distance(substrStart, substrStop)) <= _str.size());
            assert_count_in_range(size() + std::distance(substrStart, substrStop));
 
            internal_rangeInit(substrStart, substrStop, back_inserter(*this));
        }
        basic_string& append(const CharT* _s, size_type _count)
        {
            assert_count_in_range(size() + _count);

            internal_rangeInit(&_s[0], &_s[_count], back_inserter(*this));
            return *this;
        }
        basic_string& append(const CharT* _s)
        {
            size_type len = this->internal_strlen(_s);
            assert_count_in_range(size() + len);

            internal_rangeInit(&_s[0], &_s[len], back_inserter(*this));
            return *this;
        }
        template< class InputIt >
        basic_string& append(InputIt _first, InputIt _last)
        {
            assert_count_in_range(size() + std::distance(_first, _last));

            internal_rangeInit(_first, _last, back_inserter(*this));
            return *this;
        }
        basic_string& append(std::initializer_list<CharT> _ilist)
        {
            assert_count_in_range(size() + std::distance(_ilist.begin(), _ilist.end()));

            internal_rangeInit(_ilist.begin(), _ilist.end(), back_inserter(*this));
            return *this;
        }

        basic_string& operator+=(const basic_string& _str)
        {
            return append(_str);
        }
        basic_string& operator+=(CharT _ch)
        {
            push_back(_ch);
            return *this;
        }
        basic_string& operator+=(const CharT* _s)
        {
            return append(_s);
        }
        basic_string& operator+=(std::initializer_list<CharT> _ilist)
        {
            return append(_ilist);
        }

        int compare(const basic_string& _str) const noexcept
        {
            return unchecked_compare(begin(), end(), _str.begin(), _str.end());
        }
        int compare(size_type _pos1, size_type _count1, const basic_string& _str) const
        {
            assert_access_in_range(_pos1);

            return unchecked_compare(begin() + _pos1, 
                                     begin() + _pos1 + std::min(_count1, size() - _pos1), 
                                     _str.begin(), 
                                     _str.end());
        }
        int compare(size_type _pos1, size_type _count1, 
                    const basic_string& _str, size_type _pos2, size_type _count2 /*= npos*/) const
        {
            assert_access_in_range(_pos1);
            // assert_access_in_range(_pos2);

            return unchecked_compare(begin() + _pos1, 
                                     begin() + _pos1 + std::min(_count1, size() - _pos1), 
                                     _str.begin() + _pos2, 
                                     _str.begin() + _pos2 + std::min(_count2, size() - _pos2));
        }
        int compare(const CharT* _s) const
        {
            size_type len = internal_strlen(_s);

            return unchecked_compare(&_s[0], &_s[len], begin(), end());
        }
        int compare(size_type _pos1, size_type _count1, const CharT* _s) const
        {
            assert_access_in_range(_pos1);

            size_type len = internal_strlen(_s);

            return unchecked_compare(begin() + _pos1, 
                                     begin() + _pos1 + std::min(_count1, size() - _pos1), 
                                     &_s[0], 
                                     &_s[len]);
        }
        int compare(size_type _pos1, size_type _count1,
                    const CharT* _s, size_type _count2) const
        {
            assert_access_in_range(_pos1);

            return unchecked_compare(begin() + _pos1, 
                                     begin() + _pos1 + std::min(_count1, size() - _pos1), 
                                     &_s[0], 
                                     &_s[0] + _count2);
        }

        basic_string& replace(size_type _pos, size_type _count, const basic_string& _str)
        {
            assert_access_in_range(_pos);

            size_type count = std::min(_count, size() - _pos);

            unchecked_erase(begin() + _pos, begin() + (_pos + count));
            unchecked_insert_it(begin() + _pos, _str.begin(), _str.end());

            return *this;
        }
        basic_string& replace(const_iterator _first, const_iterator _last, const basic_string& _str)
        {
            assert_valid_iterator_pair(_first, _last);

            unchecked_erase(_first, _last);
            unchecked_insert_it(_first, _str.begin(), _str.end());

            return *this;
        }
        basic_string& replace(size_type _pos, size_type _count,
                              const basic_string& _str,
                              size_type _pos2, size_type _count2 = npos)
        {
            assert_access_in_range(_pos);
            // assert_access_in_range(_pos2);

            size_type count = std::min(_count , size() - _pos);
            size_type count2 = std::min(_count2, _str.size() - _pos2);

            unchecked_erase(begin() + _pos, begin() + (_pos + count));
            unchecked_insert_it(begin() + _pos, _str.begin() + _pos2, _str.begin() + (_pos2 + count2));

            return *this;
        }
        basic_string& replace(size_type _pos, size_type _count,
                              const CharT* _s, size_type _count2 )
        {
            assert_access_in_range(_pos);

            size_type count = std::min(_count , size() - _pos);

            unchecked_erase(begin() + _pos, begin() + (_pos + count));
            unchecked_insert_it(begin() + _pos, &_s[0], &_s[_count2]);

            return *this;
        }
        basic_string& replace(const_iterator _first, const_iterator _last,
                              const CharT* _s, size_type _count2)
        {
            assert_valid_iterator_pair(_first, _last);

            unchecked_erase(_first, _last);
            unchecked_insert_it(_first, &_s[0], &_s[_count2]);

            return *this;          
        }
        basic_string& replace(size_type _pos, size_type _count, const CharT* _s)
        {
            assert_access_in_range(_pos);

            size_type count = std::min(_count , size() - _pos);
            size_type len = internal_strlen(_s);

            unchecked_erase(begin() + _pos, begin() + (_pos + count));
            unchecked_insert_it(begin() + _pos, &_s[0], &_s[len]);

            return *this;        
        }
        basic_string& replace(const_iterator _first, const_iterator _last,
                              const CharT* _s)
        {
            assert_valid_iterator_pair(_first, _last);

            size_type len = internal_strlen(_s);

            unchecked_erase(_first, _last);
            unchecked_insert_it(_first, &_s[0], &_s[len]);

            return *this;   
        }
        basic_string& replace(size_type _pos, size_type _count,
                              size_type _count2, CharT _ch)
        {
            assert_access_in_range(_pos);

            size_type count = std::min(_count , size() - _pos);

            unchecked_erase(begin() + _pos, begin() + (_pos + count));
            unchecked_insert_value(begin() + _pos, _count2, _ch);          

            return *this;
        }
        basic_string& replace(const_iterator _first, const_iterator _last,
                              size_type _count2, CharT _ch)
        {
            assert_valid_iterator_pair(_first, _last);

            unchecked_erase(_first, _last);
            unchecked_insert_value(_first, _count2, _ch);          

            return *this;
        }
        template< class InputIt >
        basic_string& replace(const_iterator _first, const_iterator _last,
                              InputIt _first2, InputIt _last2)
        {
            assert_valid_iterator_pair(_first, _last);
            assert_valid_iterator_pair(_first2, _last2);

            unchecked_erase(_first, _last);
            unchecked_insert_it(_first, _first2, _last2);   

            return *this;        
        }
        basic_string& replace(const_iterator _first, const_iterator _last,
                              std::initializer_list<CharT> _ilist)
        {
            assert_valid_iterator_pair(_first, _last);
            assert_valid_iterator_pair(_ilist.begin(), _ilist.end());

            unchecked_erase(_first, _last);
            unchecked_insert_it(_first, _ilist.begin(), _ilist.end());   

            return *this;               
        }


        size_type copy(CharT* _dest, size_type _count, size_type _pos = 0) const
        {
            assert_access_in_range(_pos);

            if (_count != npos)
            {
                _count = std::min(_count, size() - _pos);
            }
            else
            {
                _count = size() - _pos;
            }

            std::copy_n(begin() + _pos, _count, &_dest[0]);

            return _count;
        }

        void resize(size_type _count)
        {
            assert_count_in_range(_count);

            unchecked_resize(_count, '\0');
        }
        void resize(size_type _count, CharT _ch)
        {
            assert_count_in_range(_count);

            unchecked_resize(_count, _ch);
        }

        void swap(basic_string& _other) noexcept
        {
            internal_swap(_other);
        }

        size_type find(const basic_string& _str, size_type _pos = 0) const
        {
            return unchecked_find(begin() + _pos, end(), _str.begin(), _str.end(), _pos + _str.size());
        }
        size_type find(const CharT* _s, size_type _pos, size_type _count) const
        {
            size_type len = internal_strlen(_s);

            return unchecked_find(begin() + _pos, end(), &_s[0], &_s[_count], (_pos + len) - _count);
        }
        size_type find(const CharT* _s, size_type _pos = 0) const
        {
            size_type len = internal_strlen(_s);

            return unchecked_find(begin() + _pos, end(), &_s[0], &_s[len], (_pos + len));
        }
        size_type find(CharT _ch, size_type _pos = 0) const noexcept
        {
            const_iterator it = std::find(begin() + _pos, end(), _ch);

            if (it != end())
            {
                return std::distance(begin(), it);
            }
            else
            {
                return npos;
            }
        }
        // basic_string substr(size_type _pos /*= 0*/, size_type _count /*npos*/) const
        // {
        //     return basic_string<50>(*this, _pos, _count);
        // }

        basic_string(pointer _pointer, size_type _storageSize)
        : m_begin(_pointer)
        , m_pos(_pointer)
        , m_capacity{_storageSize}
        {
        }

        basic_string(pointer _pointerBegin, pointer _pointerEnd, size_type _storageSize)
        : m_begin(_pointerBegin)
        , m_pos(_pointerEnd)
        , m_capacity{_storageSize}
        {
        }

        basic_string(basic_string const& _other)
        : m_begin(_other.m_begin)
        , m_pos(_other.m_pos)
        , m_capacity{_other.m_capacity}
        {
        }

    protected:

        template<typename InputIt1, typename InputIt2>
        int unchecked_compare(const InputIt1 _first1, const InputIt1 _last1, const InputIt2 _first2, const InputIt2 _last2) const
        {
            std::pair<InputIt1, InputIt2> itPair = std::mismatch(_first1, _last1, _first2, _last2);

            if((itPair.first == _last1) && (itPair.second == _last2))
            {
                return 0;
            }
            else if((itPair.first == _last1) || (*itPair.first < *itPair.second))
            {
                return -1;
            }
            else
            {
                return 1;
            }
            
        }

        template<typename InputIt1, typename InputIt2>
        size_type unchecked_find(const InputIt1 _first1, const InputIt1 _last1, const InputIt2 _first2, const InputIt2 _last2, size_type _subStrLength) const
        {
            if (_subStrLength > size()) { return npos; }

            const_iterator itPos = std::search(_first1, _last1, _first2, _last2);

            if (itPos == _last1)
            {
                return npos;
            }
            else
            {
                return std::distance(begin(), itPos);
            }
        }

        iterator unchecked_erase(const_iterator _first, const_iterator _last)
        {
            if(_first != _last)
            {
                if (_last == end())
                {
                    std::for_each(std::make_reverse_iterator(_last), std::make_reverse_iterator(_first), back_destroyer(*this)); //LCOV_EXCL_BR_LINE: We are not testing std::for_each branches
                    return end();
                }
                else
                {
                    static_cast<void>(std::rotate(to_iterator(_first), to_iterator(_last), end())); //LCOV_EXCL_BR_LINE: We are not testing std::rotate branches
                    std::for_each(std::make_reverse_iterator(_last), std::make_reverse_iterator(_first), back_destroyer(*this)); //LCOV_EXCL_BR_LINE: We are not testing std::for_each branches
                    return to_iterator(_first);
                }
            }
            else
            {
                return to_iterator(_last);
            }
        }

        void unchecked_resize(size_type _count, CharT _value)
        {
            size_type curSize = size();

            if(_count < curSize)
            {
                reverse_iterator newEnd(begin() + static_cast<difference_type>(_count)); //LCOV_EXCL_BR_LINE: We are not testing throw branches
                std::for_each(rbegin(), newEnd, back_destroyer(*this)); //LCOV_EXCL_BR_LINE: We are not testing std::for_each branches
            }
            else if(_count > curSize)
            {
                unchecked_push_back_count((_count - curSize), _value);
            }
            // else: Same size as before
        }

        class back_destroyer
        {
            public:
                back_destroyer(basic_string& _deque): m_basicString{_deque} {}     
                void operator()(const_reference _value) { static_cast<void>(_value); m_basicString.unchecked_pop_back(); };
            private:
                basic_string& m_basicString;
        };

        // -- Iterator adapter (back inserter)
        template<typename TContainer>
        class back_insert_iterator : public std::iterator<std::output_iterator_tag, void, void, void, void>
        {
            public:
                back_insert_iterator(TContainer& _container)
                : m_container{_container} { }

                back_insert_iterator& operator=(typename TContainer::value_type const _value)
                {
                    m_container.unchecked_push_back(_value);
                    return *this;
                }

                back_insert_iterator& operator*(){ return *this; }
                back_insert_iterator& operator++(){ return *this; }
                back_insert_iterator operator++(int){ return *this; }
            private:
                TContainer& m_container;
        }; 
        template<typename TContainer>
        inline back_insert_iterator<TContainer> back_inserter(TContainer& _container)
        { 
            return back_insert_iterator<TContainer>(_container); 
        }

        size_type internal_strlen(const_pointer _s) const
        {
            return __builtin_strlen(_s);
        }

        iterator unchecked_insert_value(const_iterator _pos, size_type _count, CharT _value) noexcept
        {
            iterator oldEnd{end()};
            unchecked_push_back_count(_count, _value);
            
            iterator positionToInsert{to_iterator(_pos)};
            (void) std::rotate(positionToInsert, oldEnd, end());
            return positionToInsert;
        }

        template<class InputIt>
        iterator unchecked_insert_it(const_iterator _pos, InputIt _first, InputIt _last)
        {
            iterator oldEnd{end()};
            (void) std::copy(_first, _last, back_inserter(*this));

            iterator positionToInsert{to_iterator(_pos)};
            (void) std::rotate(positionToInsert, oldEnd, end());   
            return positionToInsert;                            
        }

        template<typename InputIt, typename OutputIt>
        void internal_rangeInit(InputIt _first, InputIt _last, OutputIt _output)
        {
            assert_valid_iterator_pair(_first, _last);
            assert_count_in_range(std::distance(_first, _last));

            (void) std::copy(_first, _last, _output);
        }
        
        void internal_swap(basic_string& _other) noexcept
        {
            if(size() < _other.size())
            {
                iterator pos = std::swap_ranges(begin(), end(), _other.begin());

                assert_count_in_range(size() + std::distance(pos, _other.end()));
                unchecked_insert_it(cend(), 
                                    std::make_move_iterator(pos), 
                                    std::make_move_iterator(_other.end()));

                _other.erase(pos, _other.cend());
            }
            else
            {
                iterator pos = std::swap_ranges(_other.begin(), _other.end(), begin());

                _other.insert(_other.cend(), 
                              std::make_move_iterator(pos), 
                              std::make_move_iterator(end()));

                unchecked_erase(pos, cend());
            }
        }

        void unchecked_push_back_count(size_type _count, CharT _ch)
        {
            while (_count != 0)
            {
                unchecked_push_back(_ch);
                --_count;
            }
        }
        void unchecked_push_back(CharT _ch)
        {
            *m_pos = _ch;
            ++m_pos;
        }

        void unchecked_pop_back()
        {
            --m_pos;
        }

        iterator to_iterator(const_iterator _iterator) noexcept
        {
            return begin() + (_iterator - begin());
        }

        void assert_has_elements() const noexcept
        {
            BASIC_STRING_ASSERT(!empty());
        }

        void assert_space_left() const noexcept
        {
            BASIC_STRING_ASSERT(!(size() == max_size()));
        }

        void assert_count_in_range(size_type _count) const noexcept
        {
            BASIC_STRING_ASSERT(_count <= max_size());
        }

        void assert_access_in_range(size_type _count) const noexcept
        {
            BASIC_STRING_ASSERT(_count <= size());
        }

        template<typename InputIt>
        void assert_iterator_in_range(InputIt _it) const noexcept
        {
            BASIC_STRING_ASSERT(begin() <= _it);
            BASIC_STRING_ASSERT(_it <= end());
        }

        template<typename InputIt>
        void assert_valid_iterator_pair(InputIt _first, InputIt _last) const noexcept
        {
            BASIC_STRING_ASSERT(_first <= _last);
        }

        template<typename InputIt>
        void assert_iterator_pair_in_range(InputIt _first, InputIt _last) const noexcept
        {
            assert_iterator_in_range(_first);
            assert_iterator_in_range(_last);
            assert_valid_iterator_pair(_first, _last);
        }

    private:
        iterator const m_begin;
        iterator       m_pos;
        size_type      m_capacity;
};

template<typename CharT, typename Traits = std::char_traits<CharT>, Implementation::size_type Size>
class basic_string : public basic_string<CharT, Traits>
{
    private:
        using base = basic_string<CharT>;

    public:
        // -- Traits
        using traits_type = typename base::traits_type;
        using value_type = typename base::value_type;
        using reference = typename base::reference;
        using const_reference = typename base::const_reference;
        using pointer = typename base::pointer;
        using const_pointer = typename base::const_pointer;
        using size_type = typename base::size_type;
        using difference_type = typename base::difference_type;
        using iterator = typename base::iterator;
        using const_iterator = typename base::const_iterator;
        using reverse_iterator = typename base::reverse_iterator;
        using const_reverse_iterator = typename base::const_reverse_iterator;

        static const size_type npos = -1;

        basic_string()
        : base(m_storage.data(), Size)
        {
        }

        basic_string(size_type _count, CharT _ch)
        : basic_string()
        {
            this->assert_count_in_range(_count);
            this->unchecked_push_back_count(_count, _ch);   
        }

        basic_string(const basic_string& _other, size_type _pos)
        : basic_string()
        {
            this->internal_rangeInit(_other.begin() + _pos, _other.begin() + _other.size(), this->back_inserter(*this));
        }

        basic_string(const basic_string& _other, size_type _pos, size_type _count)
        : basic_string()
        {
            this->internal_rangeInit(_other.begin() + _pos, _other.begin() + (_count + 1u), this->back_inserter(*this));
        }

        basic_string(const CharT* _s, size_type _count)
        : basic_string()
        {
            this->internal_rangeInit(&_s[0], &_s[_count], this->back_inserter(*this));
        }

        basic_string(const CharT* _s)
        : basic_string()
        {
            size_type len = this->internal_strlen(_s);

            this->internal_rangeInit(&_s[0], &_s[len], this->back_inserter(*this));
        }

        template< class InputIt >
        basic_string(InputIt _first, InputIt _last)
        : basic_string()
        {
            this->internal_rangeInit(_first, _last, this->back_inserter(*this));
        }

        basic_string(basic_string const& _other) : basic_string(static_cast<base const&>(_other)) {}
        basic_string(base const& _other)  
        : basic_string()
        {
            this->internal_rangeInit(_other.begin(), _other.end(), this->back_inserter(*this));
        }

        basic_string(basic_string&& _other) : basic_string(std::move(static_cast<base&>(_other))) {}
        basic_string(base&& _other) noexcept
        : basic_string()
        {
            this->internal_rangeInit(std::make_move_iterator(_other.begin()), 
                                     std::make_move_iterator(_other.end()), 
                                     this->back_inserter(*this));
            _other.clear();
        }

        basic_string(std::initializer_list<CharT> _ilist)
        : basic_string()
        {
            this->internal_rangeInit(_ilist.begin(), _ilist.end(), this->back_inserter(*this));
        }

        // -- Assignment operator
        basic_string& operator=(const basic_string& _other) { return operator=(static_cast<const base&>(_other)); }
        basic_string& operator=(base const& _other)
        {
            base::operator=(_other);
            return *this;
        }

        basic_string& operator=(basic_string&& _other) { return operator=(std::move(static_cast<base&>(_other))); }
        basic_string& operator=(base&& _other)
        {
            base::operator=(std::move(_other));
            _other.clear();
            return *this;
        }

        basic_string& operator=(const CharT* _s)
        {
            base::operator=(_s);
            return *this;
        }

        basic_string& operator=(CharT _ch)
        {
            base::operator=(_ch);
            return *this;
        }

        basic_string& operator=(std::initializer_list<CharT> _ilist)
        {
            base::operator=(_ilist);
            return *this;
        }

        basic_string substr(size_type _pos=0, size_type _count=npos) const
        {
            return basic_string(*this, _pos, _count);
        }

        void swap(basic_string& _other) noexcept
        {
            this->internal_swap(_other);
        }

    private:

        std::array<CharT, Size> m_storage;
};

template<class CharT>
bool operator==(const basic_string<CharT>& _lhs,
                const basic_string<CharT>& _rhs) noexcept
{
    return ((_lhs.size() == _rhs.size()) && std::equal(_lhs.cbegin(), _lhs.cend(), _rhs.cbegin()));
}
template<class CharT>
bool operator!=(const basic_string<CharT>& _lhs,
                const basic_string<CharT>& _rhs) noexcept
{
    return !(_lhs == _rhs);
}
template<class CharT>
bool operator<(const basic_string<CharT>& _lhs,
               const basic_string<CharT>& _rhs) noexcept
{
    return std::lexicographical_compare(_lhs.begin(), _lhs.end(), _rhs.begin(), _rhs.end());
}
template<class CharT>
bool operator<=(const basic_string<CharT>& _lhs,
                const basic_string<CharT>& _rhs) noexcept
{
    return !(_rhs < _lhs);
}
template<class CharT>
bool operator>(const basic_string<CharT>& _lhs,
               const basic_string<CharT>& _rhs) noexcept
{
    return (_rhs < _lhs);
}
template<class CharT>
bool operator>=(const basic_string<CharT>& _lhs,
                const basic_string<CharT>& _rhs) noexcept
{
    return !(_lhs < _rhs);
}

template<class CharT>
bool operator==(const basic_string<CharT>& _lhs,
                const CharT*               _rhs) noexcept
{
    return ((_lhs.size() == basic_string<CharT>::traits_type::length(_rhs)) && std::equal(_lhs.cbegin(), _lhs.cend(), &_rhs[0]));
}
template<class CharT>
bool operator==(const CharT*               _lhs,
                const basic_string<CharT>& _rhs) noexcept
{
    return operator==(_rhs, _lhs);
}
template<class CharT>
bool operator!=(const basic_string<CharT>& _lhs,
                const CharT*               _rhs) noexcept
{
    return !(_lhs == _rhs);
}
template<class CharT>
bool operator!=(const CharT*               _lhs,
                const basic_string<CharT>& _rhs) noexcept
{
    return !(_rhs == _lhs);
}
template<class CharT>
bool operator<(const basic_string<CharT>& _lhs,
               const CharT*               _rhs) noexcept
{
    return std::lexicographical_compare(_lhs.begin(), _lhs.end(), &_rhs[0], &_rhs[basic_string<CharT>::traits_type::length(_rhs)]);
}
template<class CharT>
bool operator<(const CharT*               _lhs,
               const basic_string<CharT>& _rhs) noexcept
{
    return !operator<(_rhs, _lhs);
}
template<class CharT>
bool operator<=(const basic_string<CharT>& _lhs,
                const CharT*               _rhs) noexcept
{
    return !(_rhs < _lhs);
}
template<class CharT>
bool operator<=(const CharT*               _lhs,
                const basic_string<CharT>& _rhs) noexcept
{
    return !(_rhs < _lhs);
}
template<class CharT>
bool operator>(const basic_string<CharT>&  _lhs,
                const CharT*               _rhs) noexcept
{
    return (_rhs < _lhs);
}
template<class CharT>
bool operator>(const CharT*                _lhs,
                const basic_string<CharT>& _rhs) noexcept
{
    return (_rhs < _lhs);
}
template<class CharT>
bool operator>=(const basic_string<CharT>& _lhs,
                const CharT*               _rhs) noexcept
{
    return !(_lhs < _rhs);
}
template<class CharT>
bool operator>=(const CharT*               _lhs,
                const basic_string<CharT>& _rhs) noexcept
{
    return !(_lhs < _rhs);
}

}

namespace std
{

template<typename CharT>
void swap(basic_string<CharT>& _lhs,
          basic_string<CharT>& _rhs) noexcept
{
    _lhs.swap(_rhs);
}

}
