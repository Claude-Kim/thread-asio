#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <iostream>
#include <thread>
#include <vector>
#include <functional>
#include <queue>
#include <mutex>
#include <future>

class ThreadPool
{
	// thread pool vector
	std::vector<std::thread> thread_pool;
	// work to do
	std::queue<std::function<void()>> tasks;
	// synchronization
	std::mutex queue_mutex;
	std::condition_variable condition;
	bool stop_all;
public:
	ThreadPool(size_t);
	~ThreadPool();
	// future return
	template<class F, class... Args>
	auto enqueue(F&& f, Args&&... args)
		-> std::future<typename std::result_of<F(Args...)>::type>;
};

ThreadPool::ThreadPool(size_t thread_nums)
{
	std::thread t;
	for(size_t i=0; i<thread_nums; i++)
	{
		stop_all = false;
		thread_pool.emplace_back(
			// this threads loops doing task got from tasks queue
			[this]
			{
				for(;;)
				{
					std::function<void()> task;
					{
						std::unique_lock<std::mutex> lk(queue_mutex);
						// if stop_all is true or tasks have something, then lock lk and next
						// if stop_all is false and tasks is empty, then unlock lk and wait
						condition.wait(lk, [this]{return (stop_all==true || !tasks.empty());});
						if(stop_all && tasks.empty()) 
						{
							return; // lk unlock and thread will be terminated
						}
						task = std::move(tasks.front());
						tasks.pop(); // remove first element			 	
					}
					task();
				}
			}
		);
	}	
}

ThreadPool::~ThreadPool()
{
	{
		std::unique_lock<std::mutex> lock(queue_mutex);
		stop_all = true;
	}
	for(auto& t: thread_pool)
	{
		t.join();
	}
}

// add new work item to the pool
template<class F, class... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args) 
    -> std::future<typename std::result_of<F(Args...)>::type>
{
    using return_type = typename std::result_of<F(Args...)>::type;

    auto task = std::make_shared< std::packaged_task<return_type()> >(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );
        
    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(queue_mutex);

        // don't allow enqueueing after stopping the pool
        if(stop)
            throw std::runtime_error("enqueue on stopped ThreadPool");

        tasks.emplace([task](){ (*task)(); });
    }
    condition.notify_one();
    return res;
}


#endif