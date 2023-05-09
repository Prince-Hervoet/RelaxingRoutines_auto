#include "monitor.hpp"

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
                //pop block routine to wait quque
                //give context to pool
                //
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(MAX_TIMEOUT_MS));
    }
}




// std::vector<Soroutine *> temp;
// for (int i = 0; i < routines.size(); i++)
// {
//     temp.push_back(routines.front());
//     routines.pop();
// }
// sc->pushRoutines(temp);