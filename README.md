# stds::string
A static implementation of [std::basic_string](https://en.cppreference.com/w/cpp/string/basic_string) with
focus on embedded devices where the usage of a heap is not allowed.

**How to use**

You can define a stds::string as follow:

```c++
constexpr size_t MAX_SIZE = 20;
stds::string<MAX_SIZE> myString("Hello")
```

Append it:

```c++
myString += " World";
```

You can assign the stds::string to a reference which is independent 
from the maximum size and use it as usual:

```c++
stds::string<>& myStringRef = myString;
myStringRef = "Hello Universe";
```

## LICENSE

MIT License

Copyright (c) 2024 Quantum1337 (Christian Knupfer)

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

