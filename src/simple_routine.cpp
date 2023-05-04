#include "simple_routine.hpp"
#include "scheduler.hpp"

void simple_start()
{
}

Scheduler &getScheduler()
{
    static Scheduler sc;
    return sc;
}