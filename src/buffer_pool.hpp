#ifndef __CONTEXTPOOL_H__
#define __CONTEXTPOOL_H__

#include <mutex>
#include <stack>
#include <vector>
#include "soroutine.hpp"
#include "single_list_queue.hpp"

#define DEFAULT_CAPACILITY 512
#define MAX_CAPACITY 16384

class BufferPool
{
private:
    int capacity = DEFAULT_CAPACILITY; // the capacity of pool
    int remainSize = DEFAULT_CAPACILITY;

    /**
     * Why use a stack instead of a queue ?
     * Consider that using a stack allows you to use soroutines in the ready state with greater probability
     */
    std::stack<Soroutine *> qu;
    std::mutex mu;

public:
    BufferPool(int size);
    BufferPool();
    ~BufferPool();
    Soroutine *getRoutine();
    bool giveback(Soroutine *back);
    bool changeCapacity(int num);

    inline int getRemainSize()
    {
        return remainSize;
    }

    inline bool poolIsEmpty()
    {
        return !(remainSize > 0);
    }

    inline bool poolIsFull()
    {
        return qu.size() == capacity;
    }
};

#endif