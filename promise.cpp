#include <future>
#include <iostream>
#include <thread>
#include <chrono>

void calc(std::promise<int>& p, bool excp)
{
    try
    { 
        if (excp)
        { 
            throw std::out_of_range("Error : Out of range");
        }
        else
        {   
            p.set_value(42);
        }
    } 
    catch (...)
    {
        p.set_exception(std::current_exception());
    }
}

int main()
{
    std::promise<int> p;

    std::future<int> fut=p.get_future();

    std::thread t(calc, std::ref(p), false); // try true/false

    try
    {    
        std::cout << fut.get() << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    t.join();

}