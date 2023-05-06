#ifndef __CONTEXTPOOL_H__
#define __CONTEXTPOOL_H__

#include <mutex>
#include <queue>
#include <vector>
#include "soroutine.hpp"

#define DEFAULT_CAPACILITY  1024

class contextPool
{
private:
    int capacity = DEFAULT_CAPACILITY;          //Э�������Ľṹ���������
    int remainSize = DEFAULT_CAPACILITY;
    std::mutex poolMutex;                       //�̳߳���
    std::queue<Soroutine *> qu;                   //�̳߳ض���

public:
    contextPool(int size);
    contextPool();
    ~contextPool();
    Soroutine *getContext();
    std::vector<Soroutine *> getContext(int num); // ��ȡָ��������Э��������
    bool giveBacktoPool(Soroutine *back);           //����Э��������
    bool giveBacktoPool(std::vector<Soroutine *> back);

        inline int
        getRemainSize()
    {
        return remainSize;
    }
};

contextPool::contextPool(int size) : capacity(size), remainSize(size)
{
    poolMutex.lock();
    for (int i = 0; i < capacity; i++)
    {
        Soroutine *ptr = new Soroutine;
        qu.push(ptr);
    }
    poolMutex.unlock();
}

contextPool::contextPool()
{
    poolMutex.lock();
    for (int i = 0; i < capacity; i++)
    {
        Soroutine *ptr = new Soroutine;
        qu.push(ptr);
    }
    poolMutex.unlock();
}

contextPool::~contextPool()
{
    for (int i = 0; i < capacity; i++)
    {
        qu.pop();   
    }
}

Soroutine *contextPool::getContext()
{
    Soroutine *out;
    poolMutex.lock();
    
    if (!qu.empty())
    {
        out = qu.front();
        qu.pop();
        remainSize--;
    }

    poolMutex.unlock();
    return out;
}

std::vector<Soroutine *> contextPool::getContext(int num)
{
    std::vector<Soroutine *> out;
    Soroutine *ptr;
    poolMutex.lock();
    
    for (int i = 0; i < num && !qu.empty(); i++)
    {
        ptr = qu.front();
        qu.pop();
        out.emplace_back(ptr);
        remainSize--;
    }

    poolMutex.unlock();
    return out;
}

bool contextPool::giveBacktoPool(Soroutine *back)
{
    poolMutex.lock();
    qu.push(back);
    poolMutex.unlock();
    return true;
}

bool contextPool::giveBacktoPool(std::vector<Soroutine *> back)
{
    poolMutex.lock();
    for (auto x : back)
    {
        qu.push(x);
    }
    poolMutex.unlock();
}

#endif