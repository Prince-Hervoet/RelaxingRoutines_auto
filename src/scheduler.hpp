#include <vector>
#include <mutex>
#include "soroutine.hpp"
#include "single_list_queue.hpp"

class Executor;
class Scheduler
{
    friend class Soroutine;

private:
    int systemCoreSize = 0;
    std::vector<Executor *> executors;         // all executors
    SingleListQueue<Executor> freeExecutors;   // free queue
    SingleListQueue<TaskFunc> globalTaskQueue; // wait queue
    int taskSize = 0;
    int stackSize = STACK_SIZE;
    std::mutex mu;
    Soroutine *createRoutine();
    void setRoutineInfo(Soroutine *so, TaskFunc task, void *args);
    void createThread();
    void distributeRoutine();
    void checkRoutine();

public:
    Scheduler();
    void addTask(TaskFunc task);
    void addFreeExecutor(Executor *executor);
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