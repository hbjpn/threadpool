#include "ThreadPool.h"
#include <iostream>

using namespace tp;

class SampleJob : public Job
{
public:
    SampleJob(){}
    virtual ~SampleJob(){}
    
    virtual void run()
    {
        std::cout << "Sample running ... " << std::this_thread::get_id() << std::endl; std::flush(std::cout);
        std::this_thread::sleep_for(std::chrono::milliseconds(rand()%10));
    }
};

class SampleJob2 : public Job
{
public:
    SampleJob2(){}
    virtual ~SampleJob2(){}
    
    virtual void run()
    {
        std::cout << "Sample 2 running ... " << std::this_thread::get_id() << std::endl; std::flush(std::cout);
        std::this_thread::sleep_for(std::chrono::milliseconds(rand()%20));
    }
};

int main()
{
    ThreadPool tp(20);
    
    for(int i = 0; i < 100; ++i)
        tp.add(JobPtr(new SampleJob()));
    tp.start();
    
    //std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    
    for(int i = 0; i < 100; ++i)
        tp.add(JobPtr(new SampleJob2()));
       
    tp.join();
        
    return 0;
}
