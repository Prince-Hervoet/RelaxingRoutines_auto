#ifndef __SIMPLE_ROURINE_H__
#define __SIMPLE_ROURINE_H__

#include "util.hpp"

class Scheduler;
void simple_start(TaskFunc task, void *args);
// Scheduler &getScheduler(TaskFunc task);
void simple_setStack(int size);

#endif