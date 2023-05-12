#pragma once
#include <queue>
#include <mutex>
#include <ucontext.h>
#include <condition_variable>
#include "soroutine.hpp"

#ifdef __unix__
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#endif

class Scheduler;

class RoutineThread
{
    friend class Soroutine;
    friend class Scheduler;
    friend class monitor;

private:
#ifdef __unix__
    pthread_t id;
#endif

#ifdef _WIN32
    // window thread id
#endif
    bool isStart = false;
    volatile bool isAccept = false;
    Soroutine *running;
    Scheduler *sc;
    ucontext_t host;
    std::mutex mu;
    std::condition_variable cond;
    volatile uint64_t prevResumeTime = -1;
    std::queue<Soroutine *> routines;
    void resumeRoutine(Soroutine *so);
    void getFromWaitQueue();
    void stealOther();
    Soroutine *pollRoutine();
    void resumeAccept();

public:
    RoutineThread();
    RoutineThread(Scheduler *sc);
    void start();
    bool solveTimeout();
    bool getIsAccept();
    bool addRoutine(Soroutine *so);
    static void threadRunFunc(void *args);

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

    inline pthread_t getId()
    {
        return id;
    }

private:
};
