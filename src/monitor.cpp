#include "monitor.hpp"

RoutineThread *monitor::blockThread = nullptr;

void monitor::blockingHandle(int sig)
{
    RoutineThread *tmp = nullptr;

    if (blockThread != nullptr)        
    {
        tmp = blockThread;          //recorde thread pointer
        blockThread = nullptr;      //unlock spin lock
        tmp->running->status = ROUTINE_STATUS_PENDING;      //set routine state to pending, this actually send routine to queue tail
        tmp->addRoutine(tmp->running);                      
        swapcontext(&(tmp->running->context), &(tmp->host));
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
                while(blockThread != nullptr);          //spin lock when the sighandle is over
                blockThread = threads[i];
                pthread_kill(threads[i]->getId(), SIGUSR2);     //send pending sig to thread
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(MAX_TIMEOUT_MS));     //sleep until time out
    }
}

//creat the monitor thread and run 
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
