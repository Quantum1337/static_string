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

#include "../src/basic_string.hpp"
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

void Test_Constructors(void)
{
        static constexpr size_t STRING_SIZE = 200u;
    { // Default constructor
        basic_string<char, STRING_SIZE> UT_basicString;

        TEST_ASSERT_EQUAL(true, UT_basicString.empty());
        TEST_ASSERT_EQUAL(false, UT_basicString.full());
        TEST_ASSERT_EQUAL(STRING_SIZE, UT_basicString.max_size());
        TEST_ASSERT_EQUAL(0u, UT_basicString.size());
    }

    basic_string<char, STRING_SIZE> s1, s2(30, '\0');
    basic_string<char, STRING_SIZE> s3(4, '\0');
    basic_string<char, STRING_SIZE> s4("s4"), s5("s5xxx", 2), s6(3, 'a');
    basic_string<char, STRING_SIZE> s7(5, 'b'), s8(1, 'c');
    basic_string<char, STRING_SIZE> s9(s7);
    basic_string<char, STRING_SIZE> s10("s5xxx", 1, 4);

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

}

int main(int argc, char const *argv[])
{
    static_cast<void>(argc);
    static_cast<void>(argv);
    
    UNITY_BEGIN();
    RUN_TEST(Test_Constructors);
    return UNITY_END();

    return 0;
}

