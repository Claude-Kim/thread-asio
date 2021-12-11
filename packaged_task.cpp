#include <future>
#include <iostream>

int calc(bool excp)
{
    
    if (excp)
    { 
        throw std::out_of_range("Error : Out of range");
    }
    else
    {   
        return(42);
    }
    return(0);
}

int main()
{
    std::packaged_task<int(bool)> task(calc);

    std::future<int> fut=task.get_future();

    std::thread t(std::move(task), true); // try true/false

    try
    {    
        std::cout << fut.get() << std::endl;
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    t.join();

}