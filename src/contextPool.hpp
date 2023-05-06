#ifndef __CONTEXTPOOL_H__
#define __CONTEXTPOOL_H__

#include <mutex>
#include <queue>
#include <vector>
#include "soroutine.hpp"
#include "single_list_queue.hpp"

#define DEFAULT_CAPACILITY  1024
#define MAX_CAPACITY        16384

class contextPool
{
private:
    int capacity = DEFAULT_CAPACILITY;          //the capacity of pool
    int remainSize = DEFAULT_CAPACILITY;
    SingleListQueue<Soroutine> qu;              //quque of the pool

public:
    contextPool(int size);
    contextPool();
    ~contextPool();
    Soroutine *getContext();
    bool giveBacktoPool(Soroutine *back);
    bool giveBacktoPool(SingleListQueue<Soroutine> *back);
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
        return remainSize == capacity;
    }
};

contextPool::contextPool(int size) : capacity(size), remainSize(size)
{
    Soroutine *ptr = nullptr;
    for (int i = 0; i < size; i++)
    {
        ptr = new Soroutine;
        qu.add(*ptr);
    }
}

contextPool::contextPool()
{
    Soroutine *ptr = nullptr;
    for (int i = 0; i < capacity; i++)
    {
        ptr = new Soroutine;
        qu.add(*ptr);
    }
}


contextPool::~contextPool()
{
    Soroutine *ptr = nullptr;
    for (int i = 0; i < getRemainSize(); i++)
    {
        ptr = qu.poll();
        delete ptr;
    }
}

/**
 * @brief get soroutine from pool
 * 
 * @return Soroutine* 
 */
Soroutine *contextPool::getContext()
{
    Soroutine *out = nullptr;
    if (!poolIsEmpty())
    {
        out = qu.poll();
        remainSize--;
    }
    else
    {
        out = new Soroutine;            //when pool is empty alloc a class
    }
    return out;
}

/**
 * @brief 
 * 
 * @param back 
 * @return true give back succese
 * @return false when the parament is a null pointer
 */
bool contextPool::giveBacktoPool(Soroutine *back)
{
    if (back == nullptr)
    {
        return false;
    }

    if (!poolIsFull())
    {
        qu.add(*back);
        remainSize++;
    }
    else
    {
        delete back;
    }

    return true;
}

bool contextPool::giveBacktoPool(SingleListQueue<Soroutine> *back)
{
    if (back == nullptr)
    {
        return false;
    }
    
    //if pool don't have enough space to push
    if (back->getSize() > capacity - remainSize)
    {
        SingleListQueue<Soroutine> *x = back->pollMany(back->getSize() - (capacity - remainSize));
        while (x->getSize() > 0)
        {
            delete x->poll();
        }
    }
    qu.joinOther(back);
    remainSize += back->getSize();

    return true;
}

bool contextPool::changeCapacity(int num)
{
    if (num < 0 || num > MAX_CAPACITY)
    {
        return false;
    }

    int differ;
    if (num > capacity)
    {
        differ = num - capacity;
        Soroutine *ptr;
        for (int i = 0; i < differ; i++)
        {
            ptr = new Soroutine;
            qu.add(*ptr);
        }
        remainSize += differ;
    }
    else
    {
        if (remainSize > num)
        {
            differ = remainSize - num;
            for (int i = 0; i < differ; i++)
            {
                delete qu.poll();
            }
            remainSize = num;
        }
    }
    capacity = num;
    return true;
}

#endif