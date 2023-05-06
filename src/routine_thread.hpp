#include "executor.hpp"
#include <thread>

class RoutineThread
{
private:
    Executor *executor;
    bool isStarted = false;
    static void threadRunFunc(void *args);

public:
    RoutineThread();
    RoutineThread(Executor *executor);
    ~RoutineThread();
    void start();
    void stop();
};

RoutineThread::RoutineThread()
{
}

RoutineThread::RoutineThread(Executor *executor)
{
    this->executor = executor;
}

RoutineThread::~RoutineThread()
{
}

void RoutineThread::start()
{
    if (!isStarted)
    {
        return;
    }
    isStarted = true;
    std::thread(RoutineThread::threadRunFunc, this);
}

void RoutineThread::threadRunFunc(void *args)
{
    RoutineThread *rt = (RoutineThread *)args;
    // Look for it in the local queue first
    rt->executor->startRun();
    // Look for it in the global queue
    rt->executor->checkGlobalQueue();
}
