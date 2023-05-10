#include "monitor.hpp"

std::map<pthread_t, RoutineThread *> monitor::handlerMap;
void monitor::blockingHandle(int sig)
{
    RoutineThread *tmp = nullptr;
    pthread_t tid = pthread_self();
    RoutineThread *rt = monitor::handlerMap[tid];
    if (rt != nullptr)
    {
        rt->running->status = ROUTINE_STATUS_PENDING; // set routine state to pending, this actually send routine to queue tail
        rt->addRoutine(rt->running);
        swapcontext(&(rt->running->context), &(rt->host));
    }
}

monitor::monitor()
{
}

monitor::~monitor()
{
}

void monitor::startMonit(std::deque<RoutineThread *> &threads)
{
    // keep monit when the monitor thread is started
    for (;;)
    {
        for (int i = 0; i < threads.size(); i++)
        {
            // if thread is blocking
            if (threads[i]->solveTimeout())
            {
                // pull the block routine to thread routines back
                // spin lock when the sighandle is over
                // send pending sig to thread
                pthread_kill(threads[i]->getId(), SIGUSR2);
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10)); // sleep until time out
    }
}

// creat the monitor thread and run
void monitor::start(std::deque<RoutineThread *> &threads)
{
    for (int i = 0; i < threads.size(); i++)
    {
        monitor::handlerMap[threads[i]->id] = threads[i];
    }
#ifdef __unix__
    pthread_create(&id, NULL, (void *(*)(void *))monitor::startMonit, &threads);
    int res = pthread_detach(id);
    if (res != 0)
    {
        std::cout << "phread_datch failed" << std::endl;
        return;
    }

#endif
}
