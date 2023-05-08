#include <iostream>
#include "buffer_pool.hpp"

BufferPool::BufferPool(int size) : capacity(size), remainSize(size)
{
    Soroutine *ptr = nullptr;
    for (int i = 0; i < size; i++)
    {
        ptr = new Soroutine;
        qu.push(ptr);
    }
}

BufferPool::BufferPool()
{
    Soroutine *ptr = nullptr;
    for (int i = 0; i < capacity; i++)
    {
        ptr = new Soroutine;
        qu.push(ptr);
    }
}

BufferPool::~BufferPool()
{
    Soroutine *ptr = nullptr;
    for (int i = 0; i < getRemainSize(); i++)
    {
        ptr = qu.front();
        qu.pop();
        delete ptr;
    }
}

/**
 * @brief get soroutine from pool
 *
 * @return Soroutine*
 */
Soroutine *BufferPool::getRoutine()
{
    Soroutine *out = nullptr;
    if (qu.size() == 0)
    {
        return nullptr;
    }
    std::unique_lock<std::mutex> lock(mu);
    if (qu.size() > 0)
    {
        out = qu.front();
        qu.pop();
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
bool BufferPool::giveback(Soroutine *back)
{
    if (back == nullptr)
    {
        return false;
    }
    if (!poolIsFull())
    {
        std::cout << "check 2334" << std::endl;
        std::unique_lock<std::mutex> lock(mu);

        if (!poolIsFull())
        {
            qu.push(back);
            remainSize++;
        }
    }
    else
    {
        delete back;
    }
    return true;
}

bool BufferPool::changeCapacity(int num)
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
            qu.push(ptr);
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
                delete qu.front();
                qu.pop();
            }
            remainSize = num;
        }
    }
    capacity = num;
    return true;
}