// clang++ -std=c++17
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include <iterator>

std::mutex m;

using vecIntIt = std::vector<int>::iterator;

template<typename Iterator, typename T>
void func(Iterator begin, Iterator end, T& sum)
{
    for(auto it=begin; it!=end; it++ ) 
    {
        std::scoped_lock guard(m); // check this. uncomment vs comment
        sum = sum + *it;
    }
}

int main()
{
    std::vector<int> bulk;
    unsigned long sum = 0;
    unsigned long check_sum = 0;

    for (int i=1; i<=100000; i++) 
    {
        bulk.push_back(i);
        check_sum += i;
    }

    const u_int hard_thread_num = std::thread::hardware_concurrency();
   
    vecIntIt begin = bulk.begin();
    vecIntIt end = bulk.end();

    int bulk_len = std::distance(begin, end);
    int block_len = bulk_len / (hard_thread_num-1);

    std::vector<std::thread> tworks;

    for(int i=0; i<(hard_thread_num-1); i++) 
    {
        tworks.push_back(std::thread(func<vecIntIt,decltype(sum)>, begin, begin+block_len, std::ref(sum)));
        std::advance(begin,block_len);
    }
    func<vecIntIt>(begin,end,sum); // Calc remain of bulk

    for(auto& t : tworks)
    {
        t.join();
    }

    std::cout << sum << std::endl;
    std::cout << check_sum << std::endl;
}