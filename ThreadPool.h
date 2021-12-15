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
	std::vector<std::thread> tpool;
	// work to do
	std::queue<std::function<void()>> tasks;
	// synchronization
	std::mutex queue_mutex;
	std::condition_variable condition;
	bool stop;
public:
	ThreadPool(size_t);
	~ThreadPool();
	// future return
	template<class F, class... Args>
	auto enqueue(F&& f, Args&&... args)
		-> std::future<typename std::result_of<F(Args...)>::type>;
};

#endif