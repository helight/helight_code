// Copyright (c) 2014, HelightXu
// Author: Zhwen Xu<HelightXu@gmail.com>
// Created: 2014-03-09
// Description:
//
//

#include <functional>
#include <iostream>

// /// functor -> std::function object
class TestFun
{
public:
    int test(const char* wd)
    {
        std::cout << "222222222: " << wd << std::endl;
    }
    void test_func(uint64_t id)
    {
        std::cout << "class test fun: " << id << std::endl;
    }
    void test_fun2(uint64_t id)
    {
        std::cout << "1111111111: " << id << std::endl;
    }
};

typedef std::function<void(uint64_t)> CallbackFunction;

class TestFun2
{
public:
    void set_callback1(CallbackFunction& cb)
    {
        callback1 = cb;
    }
    void set_callback2(CallbackFunction& cb)
    {
        callback2 = cb;
    }
    void run()
    {
        callback1(234);
        callback2(2344444444444444);
    }

private:
    CallbackFunction callback1;
    CallbackFunction callback2;
};

void test_func(uint64_t id)
{
    std::cout << "class test fun: " << id << std::endl;
}

int main(int argc, char** argv)
{
    /// normal function -> std::function object
    TestFun p;

    TestFun2 xx;

    CallbackFunction callback1 = std::bind(&TestFun::test_func, p, std::placeholders::_1);
    CallbackFunction callback2 = std::bind(&test_func, std::placeholders::_1);
    xx.set_callback1(callback1);
    xx.set_callback2(callback2);
    xx.run();
    return 0;
}
