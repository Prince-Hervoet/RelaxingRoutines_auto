#include "soroutine.hpp"

Soroutine::Soroutine(TaskFunc task, void *args)
{
    this->task = task;
    this->args = args;
    myStack = new char[STACK_SIZE];
    alreadySize = STACK_SIZE;
    status = ROUTINE_READY;
}

Soroutine::Soroutine(TaskFunc task, void *args, int stackSize)
{
    this->task = task;
    this->args = args;
    myStack = new char[stackSize];
    alreadySize = stackSize;
    status = ROUTINE_READY;
}

void Soroutine::setStack(int newSize)
{
    if (newSize <= 0)
    {
        if (myStack)
        {
            delete[] myStack;
        }
        this->size = 0;
        alreadySize = 0;
        return;
    }
    else if (alreadySize != newSize)
    {
        if (myStack)
        {
            delete[] myStack;
        }
        myStack = new char[newSize];
        alreadySize = newSize;
        this->size = 0;
    }
}
