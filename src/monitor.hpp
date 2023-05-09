#ifndef __MONITOR_H__
#define __MONITOR_H__


#include <queue>
#include <deque>
#include <chrono>
#include <thread>
#include "scheduler.hpp"
#include "routine_thread.hpp"

class monitor
{
private:
    /* data */

public:
    void startMonit(std::deque<RoutineThread *> &threads);
    
    monitor(/* args */);
    ~monitor();
};


monitor::monitor(/* args */)
{

}

monitor::~monitor()
{
    
}


#endif