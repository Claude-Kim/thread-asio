#include <iostream>
#include <memory>
#include <mutex>
#include <thread>

class Conn
{
public:
    Conn() {
        std::cout << "Conn ctor" << std::endl;
    }
    void do_something(std::string& _msg) 
    {
        std::scoped_lock guard(m);
        // process with connection 
        std::cout << _msg << std::endl;

    }
private:
    std::mutex m;  
    std::shared_ptr<std::string> connection;
};

class X
{
private: 
    std::once_flag resource_flag;
    std::shared_ptr<Conn> resource_ptr;
public:
    X() {
        std::cout << "X ctor" << std::endl;
    }
    void init_resource()
    {
        resource_ptr.reset(new Conn);
    }
    void func(std::string& msg)
    {
        std::call_once(resource_flag, &X::init_resource, this);
        resource_ptr->do_something(msg);
    }   
};

int main()
{
    X x;
    std::string msg = "Hello";
    
    std::thread t1(&X::func, &x, std::ref(msg));
    std::thread t2(&X::func, &x, std::ref(msg));
    std::thread t3(&X::func, &x, std::ref(msg));

    t1.join();
    t2.join();
    t3.join();

}

