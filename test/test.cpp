/*
MIT License

Copyright (c) 2023 Quantum1337

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

#include "../src/string.hpp"
#include "unity/unity.h"

using namespace stds;

void setUp(void)
{
    // set stuff up here
}

void tearDown(void)
{
    // clean stuff up here
}

void test_traits() { // test char_traits
    typedef string<500>::traits_type Mytr;

    char ch[]             = "x123456789";
    Mytr::char_type* pc   = (char*) nullptr;
    Mytr::int_type* pi    = (int*) nullptr;

    pc  = pc; // to quiet diagnostics
    pi  = pi;

    Mytr::assign(ch[0], '0');
    TEST_ASSERT_EQUAL(ch[0], '0');
    TEST_ASSERT_TRUE(Mytr::eq(ch[0], ch[0]));
    TEST_ASSERT_TRUE(!Mytr::eq(ch[0], '4'));
    TEST_ASSERT_TRUE(Mytr::lt(ch[0], '4'));
    TEST_ASSERT_TRUE(!Mytr::lt(ch[0], ch[0]));
    TEST_ASSERT_TRUE(Mytr::compare("abc", "abcd", 3) == 0);
    TEST_ASSERT_TRUE(Mytr::compare("abc", "abcd", 4) < 0);
    TEST_ASSERT_EQUAL(Mytr::length(""), 0);
    TEST_ASSERT_EQUAL(Mytr::length(ch), 10);
    TEST_ASSERT_EQUAL(Mytr::find(ch, 3, '3'), nullptr);
    TEST_ASSERT_EQUAL(*Mytr::find("abcd", 4, 'd'), 'd');
    TEST_ASSERT_EQUAL_STRING(Mytr::move(ch, "abc", 0), "0123456789");
    TEST_ASSERT_EQUAL_STRING(Mytr::move(&ch[2], ch, 4), "01236789");
    TEST_ASSERT_EQUAL_STRING(Mytr::move(ch, &ch[2], 4), "0123236789");
    TEST_ASSERT_EQUAL_STRING(Mytr::copy(ch, "abc", 0), "0123236789");
    TEST_ASSERT_EQUAL_STRING(Mytr::copy(&ch[2], ch, 2), "01236789");
    TEST_ASSERT_EQUAL_STRING(Mytr::assign(ch, 2, '3'), "3301236789");
    TEST_ASSERT_EQUAL(Mytr::not_eof('e'), 'e');
    TEST_ASSERT_TRUE(Mytr::not_eof(EOF) != EOF);
    TEST_ASSERT_EQUAL(Mytr::to_char_type('x'), 'x');
    TEST_ASSERT_EQUAL(Mytr::to_int_type('x'), 'x');
    TEST_ASSERT_TRUE(Mytr::eq_int_type('x', 'x'));
    TEST_ASSERT_TRUE(Mytr::eq_int_type(EOF, EOF));
    TEST_ASSERT_TRUE(!Mytr::eq_int_type('x', EOF));
    TEST_ASSERT_EQUAL(Mytr::eof(), EOF);
}

void Test_Constructors(void)
{
    static constexpr size_t STRING_SIZE = 200u;

    string<STRING_SIZE> s1, s2(30, '\0');
    string<STRING_SIZE> s3(4, '\0');
    string<STRING_SIZE> s4("s4"), s5("s5xxx", 2), s6(3, 'a');
    string<STRING_SIZE> s7(5, 'b'), s8(1, 'c');
    string<STRING_SIZE> s9(s7);
    string<STRING_SIZE> s10("s5xxx", 1, 4);

    TEST_ASSERT_EQUAL_STRING(s1.c_str(), "");
    TEST_ASSERT_EQUAL_CHAR_ARRAY(s3.c_str(), "\0\0\0\0", 5);
    TEST_ASSERT_EQUAL_STRING(s4.c_str(), "s4");
    TEST_ASSERT_EQUAL_STRING(s5.c_str(), "s5");
    TEST_ASSERT_EQUAL_STRING(s6.c_str(), "aaa");
    TEST_ASSERT_EQUAL_STRING(s7.c_str(), "bbbbb");
    TEST_ASSERT_EQUAL_STRING(s8.c_str(), "c");
    TEST_ASSERT_EQUAL(s7.length(), 5);
    TEST_ASSERT_EQUAL_STRING(s9.c_str(), "bbbbb");
    TEST_ASSERT_EQUAL_STRING(s10.c_str(), "5xxx");

    // test assignments
    s1 = "hello";
    TEST_ASSERT_EQUAL_STRING(s1.c_str(), "hello");
    s1 = 'x';
    TEST_ASSERT_EQUAL_STRING(s1.c_str(), "x");
    s1 = s4;
    TEST_ASSERT_EQUAL_STRING(s1.c_str(), "s4");
    s1.assign("AB");
    TEST_ASSERT_EQUAL_STRING(s1.c_str(), "AB");
    s1.assign(1, 'C');
    TEST_ASSERT_EQUAL_STRING(s1.c_str(), "C");
    s1.assign(s4);
    TEST_ASSERT_EQUAL_STRING(s1.c_str(), "s4");

    // test appends
    s1 += "abc";
    TEST_ASSERT_EQUAL_STRING(s1.c_str(), "s4abc");
    s1 += 'd';
    TEST_ASSERT_EQUAL_STRING(s1.c_str(), "s4abcd");
    s1 += s4;
    TEST_ASSERT_EQUAL_STRING(s1.c_str(), "s4abcds4");
    s1 = "A";
    s1.append("BC");
    TEST_ASSERT_EQUAL_STRING(s1.c_str(), "ABC");
    s1.append(1, 'D');
    TEST_ASSERT_EQUAL_STRING(s1.c_str(), "ABCD");
    s1.append(s4);
    TEST_ASSERT_EQUAL_STRING(s1.c_str(), "ABCDs4");
    // TEST_ASSERT_EQUAL_STRING((s4 + s5).c_str(), "s4s5");
    // TEST_ASSERT_EQUAL_STRING((s4 + "s5").c_str(), "s4s5");
    // TEST_ASSERT_EQUAL_STRING(("s4" + s5).c_str(), "s4s5");
    // TEST_ASSERT_EQUAL_STRING((s4 + '5').c_str(), "s45");
    // TEST_ASSERT_EQUAL_STRING(('4' + s5).c_str(), "4s5");

    // TEST_ASSERT_TRUE((s4 + "more") == string<STRING_SIZE>("s4more"));
    // TEST_ASSERT_TRUE("more" + s4 == string<STRING_SIZE>("mores4"));
    // TEST_ASSERT_TRUE(s4 + '+' == string<STRING_SIZE>("s4+"));
    // TEST_ASSERT_TRUE('+' + s4 == string<STRING_SIZE>("+s4"));

    // test inserts
    s1 = "abc";
    s1.insert(3, "Dd");
    s1.insert(1, "BC", 1);
    s1.insert(0, "A");
    TEST_ASSERT_EQUAL_STRING(s1.c_str(), "AaBbcDd");
    s1.insert(7, 2, 'E');
    s1.insert(4, 1, 'C');
    TEST_ASSERT_EQUAL_STRING(s1.c_str(), "AaBbCcDdEE");
    s1.insert(10, s4);
    s1.insert(0, s4, 0, 1);
    TEST_ASSERT_EQUAL_STRING(s1.c_str(), "sAaBbCcDdEEs4");


    // test replaces
    s1 = "";
    s1.replace((size_t) 0, (size_t) 0, "123ab789");
    s1.replace(3, 2, "45678", 3);
    TEST_ASSERT_EQUAL_STRING(s1.c_str(), "123456789");
    s1.replace(1, 3, "xx", 2);
    s1.replace((size_t) 0, (size_t) 0, "0");
    TEST_ASSERT_EQUAL_STRING(s1.c_str(), "01xx56789");
    s1.replace(3, 1, s4, 1, 1);
    s1.replace(2, 1, s4);
    TEST_ASSERT_EQUAL_STRING(s1.c_str(), "01s4456789");

    // test copy
    char buf[10];
    s1 = "012XX";
    TEST_ASSERT_EQUAL(s1.copy(buf, sizeof(buf)), 5);
    TEST_ASSERT_EQUAL_CHAR_ARRAY(buf, "012XX", 5);
    TEST_ASSERT_EQUAL(s1.copy(buf, 3, 1), 3);
    TEST_ASSERT_EQUAL_CHAR_ARRAY(buf, "12X", 3);
    TEST_ASSERT_EQUAL(s1.copy(buf, string<>::npos, 1), 4);
    TEST_ASSERT_EQUAL_CHAR_ARRAY(buf, "12XX", 4);
    
    // test finds
    s1 = "s4s4";
    TEST_ASSERT_EQUAL(s1.find(s4), 0);
    TEST_ASSERT_EQUAL(s1.find(s4, 1), 2);
    TEST_ASSERT_EQUAL(s1.find(s4, 3), string<>::npos);
    TEST_ASSERT_EQUAL(s1.find("s4"), 0);
    TEST_ASSERT_EQUAL(s1.find("s4", 3), string<>::npos);
    TEST_ASSERT_EQUAL(s1.find("s4XX", 1, 2), 2);
    TEST_ASSERT_EQUAL(s1.find('s'), 0);
    TEST_ASSERT_EQUAL(s1.find('s', 1), 2);
    TEST_ASSERT_EQUAL(s1.find('x'), string<>::npos);

    TEST_ASSERT_EQUAL(s1.rfind(s4), 2);
    TEST_ASSERT_EQUAL(s1.rfind(s4, 1), 0);
    TEST_ASSERT_EQUAL(s1.rfind(s5, 3), string<>::npos);
    TEST_ASSERT_EQUAL(s1.rfind("s4"), 2);
    TEST_ASSERT_EQUAL(s1.rfind("s4", 3), 2);
    TEST_ASSERT_EQUAL(s1.rfind("s5", 3), string<>::npos);
    TEST_ASSERT_EQUAL(s1.rfind("s4XX", 2, 1), 2);
    TEST_ASSERT_EQUAL(s1.rfind("s4XX", 1, 3), string<>::npos);
    TEST_ASSERT_EQUAL(s1.rfind('s'), 2);
    TEST_ASSERT_EQUAL(s1.rfind('s', 2), 2);
    TEST_ASSERT_EQUAL(s1.rfind('s', 1), 0);
    TEST_ASSERT_EQUAL(s1.rfind('x'), string<>::npos);

    TEST_ASSERT_EQUAL(s1.find_first_of(s4), 0);
    TEST_ASSERT_EQUAL(s1.find_first_of(s4, 1), 1);
    TEST_ASSERT_EQUAL(s1.find_first_of(s4, 4), string<>::npos);
    TEST_ASSERT_EQUAL(s1.find_first_of("s4"), 0);
    TEST_ASSERT_EQUAL(s1.find_first_of("s4", 3), 3);
    TEST_ASSERT_EQUAL(s1.find_first_of("abs", 1, 2), string<>::npos);
    TEST_ASSERT_EQUAL(s1.find_first_of("4s1", 1, 1), 1);
    TEST_ASSERT_EQUAL(s1.find_first_of('s'), 0);
    TEST_ASSERT_EQUAL(s1.find_first_of('s', 1), 2);
    TEST_ASSERT_EQUAL(s1.find_first_of('x'), string<>::npos);

    TEST_ASSERT_EQUAL(s1.find_last_of(s4), 3);
    TEST_ASSERT_EQUAL(s1.find_last_of(s4, 1), 1);
    TEST_ASSERT_EQUAL(s1.find_last_of(s6), string<>::npos);
    TEST_ASSERT_EQUAL(s1.find_last_of("s4"), 3);
    TEST_ASSERT_EQUAL(s1.find_last_of("s4", 2), 2);
    TEST_ASSERT_EQUAL(s1.find_last_of("abs", 1, 2), string<>::npos);
    TEST_ASSERT_EQUAL(s1.find_last_of('s'), 2);
    TEST_ASSERT_EQUAL(s1.find_last_of('s', 1), 0);
    TEST_ASSERT_EQUAL(s1.find_last_of('x'), string<>::npos);

    TEST_ASSERT_EQUAL(s1.find_first_not_of(s5), 1);
    TEST_ASSERT_EQUAL(s1.find_first_not_of(s5, 2), 3);
    TEST_ASSERT_EQUAL(s1.find_first_not_of(s4), string<>::npos);
    TEST_ASSERT_EQUAL(s1.find_first_not_of("s5"), 1);
    TEST_ASSERT_EQUAL(s1.find_first_not_of("s5", 2), 3);
    TEST_ASSERT_EQUAL(s1.find_first_not_of("s4a", 1, 2), string<>::npos);
    TEST_ASSERT_EQUAL(s1.find_first_not_of('s'), 1);
    TEST_ASSERT_EQUAL(s1.find_first_not_of('s', 2), 3);
    TEST_ASSERT_EQUAL(s1.find_first_not_of('s', 4), string<>::npos);

    TEST_ASSERT_EQUAL(s1.find_last_not_of(s5), 3);
    TEST_ASSERT_EQUAL(s1.find_last_not_of(s5, 2), 1);
    TEST_ASSERT_EQUAL(s1.find_last_not_of(s4), string<>::npos);
    TEST_ASSERT_EQUAL(s1.find_last_not_of("s5"), 3);
    TEST_ASSERT_EQUAL(s1.find_last_not_of("s5", 2), 1);
    TEST_ASSERT_EQUAL(s1.find_last_not_of("s4a", 1, 2), string<>::npos);
    TEST_ASSERT_EQUAL(s1.find_last_not_of('s'), 3);
    TEST_ASSERT_EQUAL(s1.find_last_not_of('s', 2), 1);
    TEST_ASSERT_EQUAL(s1.find_last_not_of('s', 0), string<>::npos);

    // test compares
    TEST_ASSERT_TRUE(s1.compare(s1) == 0);
    TEST_ASSERT_TRUE(s1.compare(0, 6, s1) == 0);
    TEST_ASSERT_TRUE(s1.compare(2, 2, s4) == 0);
    TEST_ASSERT_TRUE(s1.compare(2, 2, s4, 0, 2) == 0);
    TEST_ASSERT_TRUE(s1.compare("s4s4") == 0);
    TEST_ASSERT_TRUE(s1.compare(0, 2, "s4") == 0);
    TEST_ASSERT_TRUE(s1.compare(0, 2, "s4", 2) == 0);
    // TEST_ASSERT_TRUE(s1.compare(2, 2, "s4xx", 0, 2) == 0);
    TEST_ASSERT_TRUE(s1.compare(s4) != 0);
    TEST_ASSERT_TRUE(s1.compare(1, 4, s1) != 0);
    TEST_ASSERT_TRUE(s1.compare(0, 4, s4, 0, 2) != 0);
    TEST_ASSERT_TRUE(s1.compare("s4s5") != 0);
    TEST_ASSERT_TRUE(s1.compare(0, 4, "s44", 1) != 0);
    // TEST_ASSERT_TRUE(s1.compare(1, 4, "s4xx", 0, 2) != 0);

    TEST_ASSERT_TRUE(s1 == s1);
    TEST_ASSERT_TRUE(s1 == "s4s4");
    TEST_ASSERT_TRUE("s4" == s4);
    TEST_ASSERT_TRUE(s1 != s4);
    TEST_ASSERT_TRUE(s1 != "xx");
    TEST_ASSERT_TRUE("s4" != s1);

    TEST_ASSERT_TRUE(s4 < s1);
    TEST_ASSERT_TRUE(s4 < "s5");
    TEST_ASSERT_TRUE("s3" < s4);
    TEST_ASSERT_TRUE(s4 <= s1);
    TEST_ASSERT_TRUE(s4 <= "s5");
    TEST_ASSERT_TRUE("s3" <= s4);

    TEST_ASSERT_TRUE(s1 > s4);
    TEST_ASSERT_TRUE("s5" > s4);
    TEST_ASSERT_TRUE(s4 > "s3");
    TEST_ASSERT_TRUE(s1 >= s4);
    TEST_ASSERT_TRUE("s5" >= s4);
    TEST_ASSERT_TRUE(s4 >= "s3");

    //Test hash compatibility
    unsigned long long hash_val1 = std::hash<string<STRING_SIZE>>()(string<STRING_SIZE>("abc"));
    unsigned long long hash_val2 = std::hash<string<>>()(string<STRING_SIZE>("abc"));
    (void) hash_val1;
    (void) hash_val2;

    { // test sto* functions
        std::size_t idx = 0;
        
        TEST_ASSERT_EQUAL(stoi(string<STRING_SIZE>("-37"), &idx), -37);
        TEST_ASSERT_EQUAL(idx, 3);
        TEST_ASSERT_EQUAL(stol(string<STRING_SIZE>("0x5a"), nullptr, 0), 0x5a);
        TEST_ASSERT_EQUAL(stoul(string<STRING_SIZE>("5c"), nullptr, 16), 0x5c);

        float fl = stof(string<STRING_SIZE>("-19.25"));
        TEST_ASSERT_EQUAL(fl, -19.25);
        TEST_ASSERT_EQUAL(stod(string<STRING_SIZE>("-19.25")), -19.25);
        TEST_ASSERT_EQUAL(stold(string<STRING_SIZE>("-19.25"), &idx), -19.25);
        TEST_ASSERT_EQUAL(idx, 6);

        long long ll = 0xffffffff;
        ll <<= 8;
        TEST_ASSERT_TRUE(stoll(string<STRING_SIZE>("0xffffffff00"), nullptr, 0) == ll);
        TEST_ASSERT_TRUE(stoull(string<STRING_SIZE>("0xffffffff00"), nullptr, 0) == (unsigned long long) ll);

        TEST_ASSERT_EQUAL_STRING(to_string((int) -23).c_str(), "-23");
        TEST_ASSERT_EQUAL_STRING(to_string((unsigned int) 23).c_str(), "23");
        TEST_ASSERT_EQUAL_STRING(to_string((long) -23).c_str(), "-23");
        TEST_ASSERT_EQUAL_STRING(to_string((unsigned long) 23).c_str(), "23");
        TEST_ASSERT_EQUAL_STRING(to_string((long long) -23).c_str(), "-23");
        TEST_ASSERT_EQUAL_STRING(to_string((unsigned long long) 23).c_str(), "23");
        TEST_ASSERT_EQUAL_STRING(to_string((long double) 23.5).c_str(), "23.5");
        TEST_ASSERT_EQUAL_STRING(to_string((float) -23.0f).c_str(), "-23");
        TEST_ASSERT_EQUAL_STRING(to_string((double) 23.2).c_str(), "23.2");
        TEST_ASSERT_EQUAL_STRING(to_string((long double) -23.0L).c_str(), "-23");

        char buffer[10];
        TEST_ASSERT_EQUAL_STRING(to_string((int) -23, &buffer[0]).c_str(), "-23");
        TEST_ASSERT_EQUAL_STRING(to_string((int) -23, &buffer[0]).c_str(), "-23");
        TEST_ASSERT_EQUAL_STRING(to_string((unsigned int) 23, &buffer[0]).c_str(), "23");
        TEST_ASSERT_EQUAL_STRING(to_string((long) -23, &buffer[0]).c_str(), "-23");
        TEST_ASSERT_EQUAL_STRING(to_string((unsigned long) 23, &buffer[0]).c_str(), "23");
        TEST_ASSERT_EQUAL_STRING(to_string((long long) -23, &buffer[0]).c_str(), "-23");
        TEST_ASSERT_EQUAL_STRING(to_string((unsigned long long) 23, &buffer[0]).c_str(), "23");
        TEST_ASSERT_EQUAL_STRING(to_string((long double) 23.5, &buffer[0]).c_str(), "23.5");
        TEST_ASSERT_EQUAL_STRING(to_string((float) -23.0f, &buffer[0]).c_str(), "-23");
        TEST_ASSERT_EQUAL_STRING(to_string((double) 23.2, &buffer[0]).c_str(), "23.2");
        TEST_ASSERT_EQUAL_STRING(to_string((long double) -23.0L, &buffer[0]).c_str(), "-23");
    }

    {
        std::initializer_list<char> init{'a', 'b', 'c'};
        string<STRING_SIZE> s11(init);
        TEST_ASSERT_EQUAL(s11.size(), 3);
        TEST_ASSERT_EQUAL(s11[2], 'c');

        string<STRING_SIZE> s12(init);
        TEST_ASSERT_EQUAL(s12.size(), 3);
        TEST_ASSERT_EQUAL(s12[2], 'c');

        s11 += init;
        TEST_ASSERT_EQUAL(s11.size(), 6);
        TEST_ASSERT_EQUAL(s11[5], 'c');

        s11 = init;
        TEST_ASSERT_EQUAL(s11.size(), 3);
        TEST_ASSERT_EQUAL(s11[2], 'c');

        s11.append(init);
        TEST_ASSERT_EQUAL(s11.size(), 6);
        TEST_ASSERT_EQUAL(s11[5], 'c');

        s11.assign(init);
        TEST_ASSERT_EQUAL(s11.size(), 3);
        TEST_ASSERT_EQUAL(s11[2], 'c');

        TEST_ASSERT_EQUAL(*s11.insert(s11.begin() + 1, init), *init.begin());
        TEST_ASSERT_EQUAL(s11.size(), 6);
        TEST_ASSERT_EQUAL(s11[2], 'b');

        s11.replace(s11.begin(), s11.begin() + 2, init);
        TEST_ASSERT_EQUAL(s11.size(), 7);
        TEST_ASSERT_EQUAL(s11[2], 'c');
    }

    // Substr
    string<STRING_SIZE> s12("abcde");
    string<STRING_SIZE> s13 = s12.substr(1 ,2);
    TEST_ASSERT_EQUAL_STRING(s13.c_str(), "bc");

    Implementation::basic_string<char, std::char_traits<char>>* ps = (string<>*) nullptr;

    ps = ps; // to quiet diagnostics

}

void Test_Container(void)
{
    typedef string<200> Mycont;
    char ch     = '\0';
    char carr[] = "abc";

    Mycont::pointer p_ptr           = (char*) nullptr;
    Mycont::const_pointer p_cptr    = (const char*) nullptr;
    Mycont::reference p_ref         = ch;
    Mycont::const_reference p_cref  = (const char&) ch;
    Mycont::value_type* p_val       = (char*) nullptr;
    Mycont::size_type* p_size       = (size_t*) nullptr;
    Mycont::difference_type* p_diff = (ptrdiff_t*) nullptr;

    p_ptr   = p_ptr;
    p_cptr  = p_cptr;
    p_ref   = p_cref;
    p_size  = p_size;
    p_diff  = p_diff;
    p_val   = p_val;

    Mycont v0;
    TEST_ASSERT_TRUE(v0.empty());
    TEST_ASSERT_EQUAL(v0.size(), 0);

    Mycont v1(5, '\0'), v1a(6, 'x'), v1b(7, 'y');
    TEST_ASSERT_EQUAL(v1.size(), 5);
    TEST_ASSERT_EQUAL(v1.end()[-1], '\0');
    TEST_ASSERT_EQUAL(v1a.size(), 6);
    TEST_ASSERT_EQUAL(v1a.end()[-1], 'x');
    TEST_ASSERT_EQUAL(v1b.size(), 7);
    TEST_ASSERT_EQUAL(v1b.end()[-1], 'y');

    Mycont v2(v1a);
    TEST_ASSERT_EQUAL(v2.size(), 6);
    TEST_ASSERT_EQUAL(*v2.begin(), 'x');

    Mycont v3(v1a.begin(), v1a.end());
    TEST_ASSERT_EQUAL(v3.size(), 6);
    TEST_ASSERT_EQUAL(*v3.begin(), 'x');

    const Mycont v4(v1a.begin(), v1a.end());
    TEST_ASSERT_EQUAL(v4.size(), 6);
    TEST_ASSERT_EQUAL(*v4.begin(), 'x');
    v0 = v4;
    TEST_ASSERT_EQUAL(v0.size(), 6);
    TEST_ASSERT_EQUAL(*v0.begin(), 'x');
    TEST_ASSERT_EQUAL(v0[0], 'x');
    TEST_ASSERT_EQUAL(v0.at(5), 'x');

    v0.reserve(12);
    TEST_ASSERT_TRUE(12 <= v0.capacity());
    v0.resize(8);
    TEST_ASSERT_EQUAL(v0.size(), 8);
    TEST_ASSERT_EQUAL(v0.end()[-1], '\0');
    v0.resize(10, 'z');
    TEST_ASSERT_EQUAL(v0.size(), 10);
    TEST_ASSERT_EQUAL(v0.end()[-1], 'z');
    TEST_ASSERT_TRUE(v0.size() <= v0.max_size());

    Implementation::basic_string<char, std::char_traits<char>>* p_cont = &v0;
    p_cont = p_cont; // to quiet diagnostics

    { // check iterators generators
        Mycont::iterator p_it(v0.begin());
        Mycont::const_iterator p_cit(v4.begin());
        Mycont::reverse_iterator p_rit(v0.rbegin());
        Mycont::const_reverse_iterator p_crit(v4.rbegin());
        TEST_ASSERT_EQUAL(*p_it, 'x');
        TEST_ASSERT_EQUAL(*--(p_it = v0.end()), 'z');
        TEST_ASSERT_EQUAL(*p_cit, 'x');
        TEST_ASSERT_EQUAL(*--(p_cit = v4.end()), 'x');
        TEST_ASSERT_EQUAL(*p_rit, 'z');
        TEST_ASSERT_EQUAL(*--(p_rit = v0.rend()), 'x');
        TEST_ASSERT_EQUAL(*p_crit, 'x');
        TEST_ASSERT_EQUAL(*--(p_crit = v4.rend()), 'x');
    }

    { // check const iterators generators
        Mycont::const_iterator p_it(v0.cbegin());
        Mycont::const_iterator p_cit(v4.cbegin());
        Mycont::const_reverse_iterator p_rit(v0.crbegin());
        Mycont::const_reverse_iterator p_crit(v4.crbegin());
        TEST_ASSERT_EQUAL(*p_it, 'x');
        TEST_ASSERT_EQUAL(*--(p_it = v0.cend()), 'z');
        TEST_ASSERT_EQUAL(*p_cit, 'x');
        TEST_ASSERT_EQUAL(*--(p_cit = v4.cend()), 'x');
        TEST_ASSERT_EQUAL(*p_rit, 'z');
        TEST_ASSERT_EQUAL(*--(p_rit = v0.crend()), 'x');
        TEST_ASSERT_EQUAL(*p_crit, 'x');
        TEST_ASSERT_EQUAL(*--(p_crit = v4.crend()), 'x');
    }

    TEST_ASSERT_EQUAL(*v0.begin(), 'x');
    TEST_ASSERT_EQUAL(*v4.begin(), 'x');

    v0.push_back('a');
    TEST_ASSERT_EQUAL(v0.end()[-1], 'a');

    v0.pop_back();
    TEST_ASSERT_EQUAL(v0.front(), 'x');
    TEST_ASSERT_EQUAL(v0.back(), 'z');

    v0.shrink_to_fit();
    TEST_ASSERT_EQUAL(v0.front(), 'x');

    {
        Mycont v5(20, 'x');
        Mycont v6(std::move(v5));
        TEST_ASSERT_EQUAL(v5.size(), 0);
        TEST_ASSERT_EQUAL(v6.size(), 20);

        Mycont v7;
        v7.assign(std::move(v6));
        TEST_ASSERT_EQUAL(v6.size(), 0);
        TEST_ASSERT_EQUAL(v7.size(), 20);

        Mycont v8;
        v8 = std::move(v7);
        TEST_ASSERT_EQUAL(v7.size(), 0);
        TEST_ASSERT_EQUAL(v8.size(), 20);
    }

    v0.assign(v4.begin(), v4.end());
    TEST_ASSERT_EQUAL(v0.size(), v4.size());
    TEST_ASSERT_EQUAL(*v0.begin(), *v4.begin());
    v0.assign(4, 'w');
    TEST_ASSERT_EQUAL(v0.size(), 4);
    TEST_ASSERT_EQUAL(*v0.begin(), 'w');
    TEST_ASSERT_EQUAL(*v0.insert(v0.begin(), 'a'), 'a');
    TEST_ASSERT_EQUAL(*v0.begin(), 'a');
    TEST_ASSERT_EQUAL(v0.begin()[1], 'w');
    TEST_ASSERT_EQUAL(*v0.insert(v0.begin(), 2, 'b'), 'b');
    TEST_ASSERT_EQUAL(*v0.begin(), 'b');
    TEST_ASSERT_EQUAL(v0.begin()[1], 'b');
    TEST_ASSERT_EQUAL(v0.begin()[2], 'a');
    TEST_ASSERT_EQUAL(*v0.insert(v0.end(), v4.begin(), v4.end()), *v4.begin());
    TEST_ASSERT_EQUAL(v0.end()[-1], v4.end()[-1]);
    TEST_ASSERT_EQUAL(*v0.insert(v0.end(), carr, carr + 3), *carr);
    TEST_ASSERT_EQUAL(v0.end()[-1], 'c');
    v0.erase(v0.begin());
    TEST_ASSERT_EQUAL(*v0.begin(), 'b');
    TEST_ASSERT_EQUAL(v0.begin()[1], 'a');
    v0.erase(v0.begin(), v0.begin() + 1);
    TEST_ASSERT_EQUAL(*v0.begin(), 'a');

    v0.clear();
    TEST_ASSERT_TRUE(v0.empty());
    v0.swap(v1);
    TEST_ASSERT_TRUE(!v0.empty());
    TEST_ASSERT_TRUE(v1.empty());
    std::swap(v0, v1);
    TEST_ASSERT_TRUE(v0.empty());
    TEST_ASSERT_TRUE(!v1.empty());
    TEST_ASSERT_TRUE(v1 == v1);
    TEST_ASSERT_TRUE(v0 < v1);
    TEST_ASSERT_TRUE(v0 != v1);
    TEST_ASSERT_TRUE(v1 > v0);
    TEST_ASSERT_TRUE(v0 <= v1);
    TEST_ASSERT_TRUE(v1 >= v0);
}

int main(int argc, char const *argv[])
{
    static_cast<void>(argc);
    static_cast<void>(argv);
    
    UNITY_BEGIN();
    RUN_TEST(Test_Constructors);
    RUN_TEST(test_traits);
    RUN_TEST(Test_Container);
    return UNITY_END();

    return 0;
}

