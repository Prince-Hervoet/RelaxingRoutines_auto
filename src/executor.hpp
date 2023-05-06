#include <iostream>
#include <cstdint>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <ucontext.h>
#include "scheduler.hpp"
#include "soroutine.hpp"
#include "single_list_queue.hpp"
#include "util.hpp"

class Executor
{
private:
    uint64_t tid;
    volatile int limit = 0;
    volatile int status = EXECUTOR_WAIT;
    Scheduler *sc;
    std::mutex mu;
    std::condition_variable cond;

    volatile uint64_t prevResumeTimestamp;
    uint64_t timeout;

    SingleListQueue<Soroutine> activeRoutines;
    Soroutine *running;

    ucontext_t host;

    // run routines
    void resumeRoutine();
    // get active routines
    Soroutine *getActiveRoutine();
    static void taskRunningFunc(Executor *executor);

public:
    void setRunning()
    {
        mu.lock();
        status = EXECUTOR_RUNNING;
        mu.unlock();
    }
    void setWait()
    {
        mu.lock();
        status = EXECUTOR_WAIT;
        mu.unlock();
    }

    void setPending()
    {
        mu.lock();
        status = EXECUTOR_PENDING;
        mu.unlock();
    }

    int getSize()
    {
        
    }

    Soroutine *getBuffer();
    bool addRoutine(Soroutine *routine);
    bool isTimeout();
    std::vector<Soroutine *> catchRoutines(int count);
};