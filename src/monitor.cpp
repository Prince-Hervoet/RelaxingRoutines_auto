#include "monitor.hpp"

RoutineThread *monitor::blockThread = nullptr;
ucontext_t temp;

void monitor::blockingHandle(int sig)
{
    RoutineThread *tmp = nullptr;

    if (blockThread != nullptr)
    {
        tmp = blockThread;
        blockThread = nullptr;
        tmp->running->status = ROUTINE_STATUS_PENDING;
        tmp->addRoutine(tmp->running);
        swapcontext(&(tmp->running->context), &(tmp->host));
        std::cout << "wce" << std::endl;
    }
}

monitor::monitor()
{
    blockThread = nullptr;
}

monitor::~monitor()
{
}

void monitor::startMonit(std::deque<RoutineThread *> &threads)
{
    //keep monit when the monitor thread is started
    for (;;)
    {
        for (int i = 0; i < threads.size(); i++)
        {
            //if thread is blocking 
            if (threads[i]->solveTimeout())
            {
                //pull the block routine to thread routines back
                while(blockThread != nullptr);
                blockThread = threads[i];
                pthread_kill(threads[i]->getId(), SIGUSR2);
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(MAX_TIMEOUT_MS));
    }
}

void monitor::start(std::deque<RoutineThread *> &threads)
{
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

// std::vector<Soroutine *> temp;
// for (int i = 0; i < routines.size(); i++)
// {
//     temp.push_back(routines.front());
//     routines.pop();
// }
// sc->pushRoutines(temp);