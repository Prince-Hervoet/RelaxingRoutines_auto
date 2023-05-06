#ifndef __EXECUTOR_H__
#define __EXECUTOR_H__

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
    uint64_t tid; // thread id
    uint64_t timeout;
    Scheduler *sc; // its scheduler
    std::mutex mu;
    std::condition_variable cond;
    volatile int status = EXECUTOR_WAIT;      // the status of this thread
    volatile uint64_t prevResumeTimestamp;    // lastUpdateAt -- judge block
    SingleListQueue<Soroutine> localRoutines; // a local queue in this thread
    Soroutine *running;
    ucontext_t host;

    void resumeRoutine();          // run routines
    Soroutine *getActiveRoutine(); // get active routines
    static void taskRunningFunc(Executor *executor);

public:
    int getSize()
    {
        return localRoutines.getSize();
    }
    bool addRoutine(Soroutine *routine);
    bool isTimeout();
    std::vector<Soroutine *> catchRoutines(int count);
};
#endif