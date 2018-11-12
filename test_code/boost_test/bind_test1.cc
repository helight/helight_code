#include <stdio.h>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <functional>

class TestClass2
{
public:
    TestClass2(){}
    ~TestClass2(){}

    void fun1()
    {
        printf("TestClass2 fun1() ....\n");
    }
    void fun2(int i)
    {
        printf("TestClass2 fun2(%d) ....\n", i);
    }
};


class TestClass
{
public:
    TestClass()
    {
        f1 = boost::bind(&TestClass2::fun1, tt);
        f2 = boost::bind(&TestClass2::fun2, tt, _1); 
    }
    ~TestClass(){}
    void fun1()
    {
        printf("fun1()\n");
    }
    void fun2(int i)
    {
        printf("fun2(%d)\n", i);
    }

    void testit()
    {
        f1();
        f2(111);
    }

private:
    boost::function<void ()> f1;
    boost::function<void (int)> f2;
    TestClass2 *tt;
};

int main(int argc, char* argv[]) {
    TestClass mc;
    mc.testit();
    return 0;
}
