#ifndef __MONITOR_H__
#define __MONITOR_H__

#include <queue>
#include <deque>
#include <chrono>
#include <thread>
#include <iostream>
#include <map>
#include "routine_thread.hpp"

class monitor
{
public:
    static std::map<pthread_t, RoutineThread *> handlerMap;

private:
    /* data */
    pthread_t id;

public:
    static void startMonit(std::deque<RoutineThread *> &threads);
    void start(std::deque<RoutineThread *> &threads);
    void start();

    static void blockingHandle(int sig);

    monitor();
    ~monitor();
};

#endif