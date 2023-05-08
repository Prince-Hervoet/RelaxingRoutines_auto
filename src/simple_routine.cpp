#include "simple_routine.hpp"
#include "scheduler.hpp"
#include <iostream>

void simple_start(TaskFunc task, void *args)
{
    // std::cout << "check 1" << std::endl;
    Scheduler &s = Scheduler::getInstance();
    // std::cout << "check 3" << std::endl;
    s.addTask(task, args);
}