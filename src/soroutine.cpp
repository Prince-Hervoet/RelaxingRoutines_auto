#include "soroutine.hpp"

Soroutine::Soroutine(TaskFunc task, void *args)
{
    this->task = task;
    this->args = args;
    myStack = new char[STACK_SIZE];
    hasSize = STACK_SIZE;
    status = ROUTINE_READY;
}

Soroutine::Soroutine(TaskFunc task, void *args, int stackSize)
{
    this->task = task;
    this->args = args;
    myStack = new char[stackSize];
    hasSize = stackSize;
    status = ROUTINE_READY;
}
