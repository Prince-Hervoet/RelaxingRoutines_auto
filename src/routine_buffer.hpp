#include <list>
#include "soroutine.hpp"

template <typename T>
class RoutineBuffer
{
private:
    std::list<T *> buffer;
    int limit = 16;

public:
    RoutineBuffer()
    {
    }

    RoutineBuffer(int limit)
    {
        this->limit = limit;
    }

    ~RoutineBuffer()
    {
        for (auto it = buffer.begin(); it != buffer.end(); it++)
        {
            delete (*it);
        }
        buffer.clear();
    }

    T *getOne()
    {
        if (buffer.size() == 0)
        {
            return nullptr;
        }
        T *t = buffer.front();
        buffer.pop_front();
        return t;
    }

    void giveBack(Soroutine *so)
    {
        if (!so)
        {
            return;
        }
        buffer.push_back(so);
    }

    int getSize()
    {
        return buffer.size();
    }
};