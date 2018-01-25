# Conveyor
It's pure C++17 based runtime and compiletime conveyor implementation.

Runtime implementation based on `std::any` type to provide type result type
evolution.

Compiletime implementation uses template magic to deduct result type to
prevent `std::any` overhead, cache misses cause of using dynamic memory for 
operators and etc.

## Build
1. Clone repo: `git clone https://github.com/Megaxela/Conveyor`
1. Go into repo: `cd Conveyor`
1. Init and update submodule if you need tests and benchmark: `git submodule init && git submodule update`
1. Create build folder: `mkdir build`
1. Go into build folder: `cd build`
1. Generate build file for your compiler: `cmake ..` (or `cmake -DCONVEYOR_BUILD_TESTS=On -DCONVEYOR_BUILD_BENCHMARK=On .. ` if you want to build tests and benchmarks)
1. Build library: `cmake --build .`

Also, you can use cmake `add_subdirectory` command to add conveyor as dependency.
Include directories will be included automatically.

## Examples

**Usage of dynamic Belt**

With dynamic belt you can change conveyor pipe in runtime with `Operator::Belt` methods.

```cpp
#include <Conveyor/Belt.hpp>
#include <Conveyor/Operator.hpp>

class AddOperator : public Conveyor::Operator
{
public:
    std::any execute(const std::any& arg) override
    {
        return std::any_cast<int>(arg) + 5;
    }
};

class ToStringOperator : public Conveyor::Operator
{
public:
    std::any execute(const std::any& arg) override
    {
        return std::to_string(std::any_cast<int>(arg));
    }
};

int main(int argc, char** argv)
{
    Conveyor::Belt belt;
    
    belt.addOperator(std::make_shared<AddOperator>());
    belt.addOperator(std::make_shared<AddOperator>());
    belt.addOperator(std::make_shared<ToStringOperator>());
    
    std::string result = std::any_cast<std::string>(belt.execute(20));
    
    // Result: 30
    std::cout << "Result: " << result << std::endl;

    return 0;
}

```

**Usage of static Belt**
```cpp
#include <Conveyor/StaticBelt.hpp>

class AddOperator
{
public:
    int execute(int arg)
    {
        return arg + 5;
    }
};

class ToStringOperator
{
public:
    std::string execute(int arg)
    {
        return std::to_string(arg);
    }
};

int main(int argc, char** argv)
{
    Conveyor::StaticBelt<
        AddOperator,
        AddOperator,
        ToStringOperator
    > belt;

    // Actually return type of `execute` method there is std::string.
    // It deduces return type out of last template operator.
    std::string result = belt.execute(20);

    // Result: 30
    std::cout << "Result: " << result << std::endl;

    return 0;
}
```

## LICENSE

<img align="right" src="http://opensource.org/trademarks/opensource/OSI-Approved-License-100x137.png">

Library is licensed under the [MIT License](https://opensource.org/licenses/MIT) 

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