#pragma once
#include <queue>
#include <mutex>
#include <ucontext.h>
#include <condition_variable>
#include "soroutine.hpp"

class Scheduler;

class RoutineThread
{
    friend class Soroutine;

private:
    bool isStart = false;
    volatile bool isAccept = false;
    Soroutine *running;
    Scheduler *sc;
    ucontext_t host;
    std::mutex mu;
    std::condition_variable cond;
    volatile uint64_t prevResumeTime;
    std::queue<Soroutine *> routines;
    void resumeRoutine(Soroutine *so);
    void getFromWaitQueue();
    void stealOther();
    Soroutine *pollRoutine();

public:
    static void threadRunFunc(void *args);
    void start();
    bool getIsAccept();
    void addRoutine(Soroutine *so);
    void setBlock()
    {
        isAccept = false;
    }

    Soroutine *getRunning()
    {
        return running;
    }

    Scheduler *getSc()
    {
        return sc;
    }
};