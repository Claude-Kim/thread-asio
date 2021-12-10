#include <iostream>
#include <future>

int calc_once()
{
    std::cout << "Starting calculation" << std::endl;
    std::this_thread::sleep_for(std::chrono::microseconds(1000000));
    return 42;
}

void sol(std::future<int>& answer)
{
    std::cout << "Waiting calculation" << std::endl;
    answer.wait(); // invoke calc_once in async
    std::cout << "answer is " << answer.get() << "." << std::endl;
}

void foo()
{
    std::cout << "I'm foo." << std::endl;
}

int main()
{
    // practice future, async
    
    std::future<int> answer=std::async(std::launch::deferred, calc_once);

    std::thread t_sol(sol, std::ref(answer));
    t_sol.join();

    // practice packaged_task

    std::packaged_task<int()> task(calc_once);

    std::future<int> res=task.get_future();

    task();

    std::cout << res.get() << std::endl;

}
