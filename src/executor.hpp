#include <iostream>
#include <cstdint>
#include <mutex>
#include <condition_variable>
#include <vector>
#include "scheduler.hpp"
#include "soroutine.hpp"
#include "single_list_queue.hpp"
#include "routine_buffer.hpp"
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
    RoutineBuffer<Soroutine> rb;
    Soroutine *running;
    void resumeRoutine();
    static void taskRunningFunc(Executor *executor);
    Soroutine *getActiveRoutine();

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
        return rb.getSize();
    }

    Soroutine *getBuffer();
    bool addRoutine(Soroutine *routine);
    bool isTimeout();
    std::vector<Soroutine *> catchRoutines(int count);
};