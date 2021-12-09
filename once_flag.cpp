#include <iostream>
#include <memory>
#include <mutex>
#include <thread>

class X
{
private:
    std::string msg;
    std::mutex m;
public:
    X() = default;
    void do_something(std::string _msg) {
        std::scoped_lock guard(m);
        msg = _msg;
    };
};

std::shared_ptr<X> resource_ptr;
std::once_flag resource_flag;

void init_resource()
{
    resource_ptr.reset(new X);
}

void func(std::string& msg)
{
    std::call_once(resource_flag, init_resource);

    resource_ptr->do_something(msg);
}

int main()
{
    std::string msg = "Hello";

    std::thread t1(func, std::ref(msg));
    std::thread t2(func, std::ref(msg));
    std::thread t3(func, std::ref(msg));

    t1.join();
    t2.join();
    t3.join();
}

