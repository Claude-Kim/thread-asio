#include <iostream>
#include <future>

class X
{
    int num;
    std::string msg;
public:
    X() 
    {
        num=0;
        msg="";
        std::cout << "default ctor" << std::endl;
    }
    X(const X& _x)
    {
        num=_x.num;
        msg=_x.msg;
        std::cout << "copy ctor" << std::endl;
    }
    void foo(int num, std::string const& msg)
    {
        std::cout << num << msg << std::endl;
    };
    std::string bar(std::string const& msg)
    {
        return msg;
    };
};

int main()
{
    X x; // default ctor

    std::future<void> f1=std::async(&X::foo, x, 42, "hello"); // copy ctor 3 times, tmpx->foo(42, "hello")
    std::future<std::string> f2=std::async(&X::bar, &x, "goodbye"); // p->bar("goodbye")

    std::cout << "f2 is " << f2.get() << "." << std::endl;
}
