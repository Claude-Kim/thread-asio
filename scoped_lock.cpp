#include <iostream>
#include <mutex>
#include <vector>
#include <thread>

class X
{
public:
    X() = default;
    ~X() = default;
    X(const std::string _text): data(_text) {}
    X(X&& rvalue){
        data = std::move(rvalue.data);
    }
    std::string& print()
    {
        return data;
    }
    std::string data;
    std::mutex m;
};

void swap(X& lhs, X& rhs)
{
    if(&lhs==&rhs)
        return;
    std::scoped_lock guard(lhs.m, rhs.m);

/*  
    std::unique_lock<std::mutex> lock_a(lhs.m, std::defer_lock);
    std::unique_lock<std::mutex> lock_b(rhs.m, std::defer_lock);
    std::lock (lock_a, lock_b);
*/

/*
    std::lock(lhs.m, rhs.m);
    std::lock_guard<std::mutex> lock_a(lhs.m, std::adopt_lock);
    std::lock_guard<std::mutex> lock_b(rhs.m, std::adopt_lock);
*/
    std::swap(lhs.data, rhs.data);
}

void swap_vector(std::vector<X>& lhs_vec, std::vector<X>& rhs_vec)
{
    std::vector<X>::iterator it_lhs=lhs_vec.begin();
    std::vector<X>::iterator it_rhs=rhs_vec.begin();

    for(; it_lhs < lhs_vec.end(); it_lhs++ ) {
        swap(*it_lhs, *it_rhs);
        it_rhs++;
    }
}

int main()
{
    std::vector<X> rv;
    std::vector<X> bv;

    for(int i=0; i<100000; i++)
    {
        rv.push_back(X("RED"));
        bv.push_back(X("BLUE"));
    }
    
    std::thread t1(swap_vector, std::ref(rv), std::ref(bv));
    std::thread t2(swap_vector, std::ref(rv), std::ref(bv));
    std::thread t3(swap_vector, std::ref(rv), std::ref(bv));

    t1.join();
    t2.join();
    t3.join();

}