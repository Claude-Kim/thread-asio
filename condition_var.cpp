#include <iostream>
#include <condition_variable>
#include <queue>
#include <string>
#include <thread>

std::mutex mut;
std::condition_variable data_cond;

class chunk
{
private:    
    std::string data;
public:
    chunk()=delete;
    chunk(const chunk& )=default; 
    chunk(std::string const _msg): data(_msg) {}
    std::string& get()
    {
        return data;
    } 
};
std::queue<chunk> store;
int test_loop_num = 10;

bool more_data_to_prepare()
{
    return (test_loop_num < 0)? false : true;                       
}

chunk prepare_data()
{   
    std::string msg;
    if (test_loop_num <= 0)
        msg = "==EOF==";
    else
        msg = "dummy";
    test_loop_num--;
    chunk D(msg); // chunk(std::string const _msg)
    std::this_thread::sleep_for(std::chrono::microseconds(100000));
    return D; //chunk(const chunk& )
} 

bool is_last_chunk(chunk data)
{
    if (data.get() == "==EOF==")
        return true;
    else   
        return false; 
}

void process(chunk data)
{
    std::cout << data.get() << ":" << std::this_thread::get_id() << std::endl;
}

void data_preparation_thread()
{
    while(more_data_to_prepare())
    {
        chunk const data=prepare_data();
        {
            std::lock_guard<std::mutex> lk(mut);
            store.push(data); // chunk(const chunk& )
        }
        data_cond.notify_one(); // notify after unlock
    }
}

void data_processing_thread()
{
    while(true)
    {
        std::unique_lock<std::mutex> lk(mut); // unique_lock for passing lk 
        data_cond.wait(lk,[]{return !store.empty();}); // if sore in not empty, lock lk and go
                                                       // if store is empty, unlock lk and wait                                            
        chunk data=store.front();
        store.pop();
        lk.unlock(); // unlock for process data 

        process(data);
        if(is_last_chunk(data))
            break;
    }
}

int main()
{
    std::thread p1(data_processing_thread);
    std::thread p2(data_processing_thread);

    std::thread c1(data_preparation_thread);
   
    p1.join();
    p2.join();
    c1.join();
}