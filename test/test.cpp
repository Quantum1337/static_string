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
#include "../src/hash.hpp"
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
    { // Default constructor
        string<STRING_SIZE> UT_basicString;

        TEST_ASSERT_EQUAL(true, UT_basicString.empty());
        TEST_ASSERT_EQUAL(false, UT_basicString.full());
        TEST_ASSERT_EQUAL(STRING_SIZE, UT_basicString.max_size());
        TEST_ASSERT_EQUAL(0u, UT_basicString.size());
    }

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

    //Test hash compatibility
    size_t hash_val = std::hash<string<>>()(string<STRING_SIZE>("abc"));
    (void) hash_val;
}

int main(int argc, char const *argv[])
{
    static_cast<void>(argc);
    static_cast<void>(argv);
    
    UNITY_BEGIN();
    RUN_TEST(Test_Constructors);
    RUN_TEST(test_traits);
    return UNITY_END();

    return 0;
}

