#include "util.hpp"

class Scheduler;
void simple_start(TaskFunc task, void *args);
// Scheduler &getScheduler(TaskFunc task);
void simple_setStack(int size);