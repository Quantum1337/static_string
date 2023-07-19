
#include "Implementation/basic_string_Types.hpp"
#include "Implementation/basic_string_Iterator.hpp"
#include "Implementation/basic_string_Assert.hpp"
#include <array>

namespace stds
{

template<typename CharT, Implementation::size_type=static_cast<Implementation::size_type>(0)>
class basic_string;

template<typename CharT>
class basic_string<CharT>
{
    public:
        // -- Traits
        // using traits_type  = Traits
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

        const_pointer c_str() const noexcept { *m_pos = '\0'; return &(*m_begin); }

        reference operator[](size_type _pos) { return *(begin() += _pos); }
        const_reference operator[](size_type _pos) const { return *(cbegin() += _pos); }

        reference front() { return *begin(); }
        const_reference front() const { return *cbegin(); }

        reference back() { return *std::prev(end()); }
        const_reference back() const { return *std::prev(cend()); }

        // -- Iterators
        iterator begin() noexcept { return m_begin; };
        const_iterator begin() const noexcept { return m_begin; };
        const_iterator cbegin() const noexcept { return m_begin; };

        iterator end() noexcept { return m_pos; };
        const_iterator end() const noexcept { return m_pos; };
        const_iterator cend() const noexcept { return m_pos; };

        reverse_iterator rbegin() noexcept { return reverse_iterator(m_begin); };
        const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(m_begin); };
        const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(m_begin); };

        reverse_iterator rend() noexcept { return reverse_iterator(m_pos); };
        const_reverse_iterator rend() const noexcept { return const_reverse_iterator(m_pos); };
        const_reverse_iterator crend() const noexcept { return const_reverse_iterator(m_pos); };

        // -- Capacity
        size_type max_size() const { return m_capacity; };
        size_type size() const { return std::distance(m_begin, m_pos); };
        size_type length() const { return std::distance(m_begin, m_pos); };
        bool empty() const { return (m_begin == m_pos); };
        bool full() const { return (static_cast<size_type>(std::distance(m_begin, m_pos)) == m_capacity); };

        basic_string(pointer _pointer, size_type _storageSize)
        : m_begin(_pointer)
        , m_pos(_pointer)
        , m_capacity{_storageSize}
        {
        }
    
    protected:

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

        size_type internal_strlen(const_pointer s)
        {
            size_type count = 0u;
            while(*s != '\0')
            {
                count++;
                s++;
            }
            return count;
        }

        template<typename InputIt, typename OutputIt>
        void internal_rangeInit(InputIt _first, InputIt _last, OutputIt _output)
        {
            assert_valid_iterator_pair(_first, _last);
            assert_count_in_range(std::distance(_first, _last));

            (void) std::copy(_first, _last, _output);
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

        void assert_count_in_range(size_type _count) const noexcept
        {
            BASIC_STRING_ASSERT(_count <= max_size());
        }

        template<typename InputIt>
        void assert_valid_iterator_pair(InputIt _first, InputIt _last) const noexcept
        {
            BASIC_STRING_ASSERT(_first <= _last);
        }

    private:
        iterator const m_begin;
        iterator       m_pos;
        size_type      m_capacity;
};

template<typename CharT, Implementation::size_type Size>
class basic_string final : public basic_string<CharT>
{
    private:
        using base = basic_string<CharT>;

    public:
        // -- Traits
        // using traits_type = typename base::traits_type;
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

        basic_string(const basic_string& _other)
        : basic_string()
        {
            this->internal_rangeInit(_other.begin(), _other.end(), this->back_inserter(*this));
        }

        basic_string(basic_string&& _other) noexcept
        : basic_string()
        {
        }

        basic_string(std::initializer_list<CharT> _ilist)
        : basic_string()
        {
            this->internal_rangeInit(_ilist.begin(), _ilist.end(), this->back_inserter(*this));
        }

    private:

        std::array<CharT, Size> m_storage;
};

}
