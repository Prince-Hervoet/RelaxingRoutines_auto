#include <vector>
#include <mutex>
#include "util.hpp"

class Executor;
class Scheduler
{
private:
    std::vector<Executor *> executors;
    int taskSize = 0;
    int stackSize = STACK_SIZE;
    std::mutex mu;
    Soroutine *createRoutine();
    void createThread();
    void distributeRoutine();
    void checkRoutine();

public:
    void addTask(TaskFunc task);
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
};