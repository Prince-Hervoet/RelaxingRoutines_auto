#include <iostream>
#include <cstdint>
#include <mutex>
#include <condition_variable>
#include <vector>
#include "scheduler.hpp"
#include "soroutine.hpp"
#include "single_list_queue.hpp"

const int EXECUTOR_STOP = 0;
const int EXECUTOR_RUNNING = 1;
const int EXECUTOR_PENDING = 2;

class Executor
{
private:
    uint64_t tid;
    volatile int limit = 0;
    volatile int status = EXECUTOR_STOP;
    Scheduler *sc;
    std::mutex mu;
    std::condition_variable cond;
    uint64_t prevResumeTimestamp;
    uint64_t timeout;
    SingleListQueue<Soroutine> soroutines;
    Soroutine *running;
    void resumeRoutine();

public:
    void start();
    void stop();
    void pending();
    bool addRoutine(Soroutine *routine);
    bool addRoutines(std::vector<Soroutine *> &routines);
    bool isTimeout();
    std::vector<Soroutine *> catchRoutines(int count);
};