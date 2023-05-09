#pragma once
#include <queue>
#include <deque>
#include <mutex>
#include <vector>
#include "soroutine.hpp"
#include "routine_thread.hpp"
#include "buffer_pool.hpp"
#include "monitor.hpp"

class Scheduler
{
    friend class Soroutine;
    friend class RoutineThread;

private:
    std::deque<RoutineThread *> rts; // save some routineThreads
    std::queue<Soroutine *> waitQueue;
    BufferPool *routinePool; // routines buffer
    std::mutex mu;
    uint64_t incrementId = 0; // set routine id
    monitor *mo;
    Soroutine *createRoutine(TaskFunc task, void *args); // create a routine
    void givebackRoutine(Soroutine *so);
    void createRoutineThread(Soroutine *so);
    void pushRoutines(std::vector<Soroutine *> &routines);
    std::vector<Soroutine *> &pollRoutines(int count);

public:
    Scheduler();
    static Scheduler &getInstance();
    void addTask(TaskFunc task, void *args);
    int getRoutineSize();
    void boostrap();
    void stop();
};