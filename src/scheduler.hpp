#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include <vector>
#include <mutex>
#include <semaphore.h>
#include "soroutine.hpp"
#include "single_list_queue.hpp"

class Executor;
class Scheduler
{
private:
    int systemCoreSize = 0;
    std::vector<Executor *> executors;          // all executors
    SingleListQueue<Soroutine> globalTaskQueue; // wait queue
    int taskSize = 0;
    int stackSize = STACK_SIZE;
    std::mutex mu;
    sem_t sem;
    Soroutine *createRoutine();
    void setRoutineInfo(Soroutine *so, TaskFunc task, void *args);
    void createThread();
    void distributeRoutine();
    void checkRoutine();

public:
    Scheduler();
    void addTask(TaskFunc task, void *args);
    void setStackSize(int size)
    {
        mu.lock();
        if (size > 0)
        {
            this->stackSize = size;
        }
        mu.unlock();
    }

    int getStackSize()
    {
        return stackSize;
    }

    int getTaskSize()
    {
        return taskSize;
    }

    int getThreadSize()
    {
        return executors.size();
    }

    int getTaskQueueSize()
    {
        return globalTaskQueue.getSize();
    }
};

#endif