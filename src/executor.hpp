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
    Soroutine *running;
    ucontext_t host;
    std::condition_variable cond;
    volatile int status = EXECUTOR_WAIT;      // the status of this thread
    volatile uint64_t prevResumeTimestamp;    // lastUpdateAt -- judge block
    SingleListQueue<Soroutine> localRoutines; // a local queue in this thread

    void resumeRoutine();          // run routines
    Soroutine *getActiveRoutine(); // get active routines

public:
    bool isTimeout();
    void startRun();
    void checkGlobalQueue();
    bool addToLocalQueue(Soroutine *routine);
    SingleListQueue<Soroutine> *catchRoutines(int count);

    int getSize()
    {
        return localRoutines.getSize();
    }
};