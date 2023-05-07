#include "simple_routine.hpp"
#include "scheduler.hpp"

void simple_start(TaskFunc task, void *args)
{
    Scheduler &s = Scheduler::getInstance();
    s.addTask(task, args);
}