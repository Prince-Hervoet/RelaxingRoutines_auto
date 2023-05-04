#include "routine_pool.hpp"

Soroutine *RoutinePool::get()
{
}

void RoutinePool::giveBack(Soroutine *so)
{
    if (!so)
    {
        return;
    }
    so->setTask(nullptr, nullptr);
    so->setStatus(ROUTINE_INIT);
    so->setStack(0);
    this->freeRoutines.push_back(so);
}