#pragma once
#include <mutex>

template <typename T>
class ListNode
{
public:
    ListNode *next = nullptr;
    T *data;
};

template <typename T>
class SingleListQueue
{
private:
    int size = 0;
    int limit = 0;
    ListNode<T> *head;
    ListNode<T> *tail;

public:
    SingleListQueue()
    {
    }

    ~SingleListQueue()
    {
        ListNode<T> *run = head;
        while (run)
        {
            ListNode *temp = run->next;
            delete run;
            run = temp;
        }
    }

    int getSize()
    {
        return size;
    }

    void add(T &t)
    {
        ListNode<T> *node = new ListNode<T>();
        node->data = &t;
        std::unique_lock<std::mutex> lock(mu);
        if (!head || !tail)
        {
            head = node;
            tail = head;
            return;
        }
        tail->next = node;
        tail = node;
        size += 1;
    }

    T *poll()
    {
        if (size == 0)
        {
            return nullptr
        }
        std::unique_lock<std::mutex> lock(mu);
        ListNode<T> *temp = head->next;
        ListNode<T> *target = head;
        T *t = target->data;
        head->next = nullptr;
        head = temp;
        size -= 1;
        delete target;
        return t;
    }

    SingleListQueue<T> *pollMany(int need)
    {
        if (need <= 0)
        {
            return nullptr;
        }
        std::unique_lock<std::mutex> lock(mu);
        ListNode<T> *run = head;
        SingleListQueue *sq = nullptr;
        while (run->next && need > 0)
        {
            run = run->next;
            need -= 1;
        }
        ListNode<T> *temp = head;
        head = run->next;
        sq = new SingleListQueue();
        sq->head = temp;
        sq->tail = run;
        if (!head)
        {
            tail = nullptr;
        }
        size = need > size ? 0 : size - need;
        return sq;
    }

    void joinOther(SingleListQueue<T> *sq)
    {
        if (!sq)
        {
            return;
        }
        std::unique_lock<std::mutex> lock(mu);
        if (head)
        {
            tail->next = sq->head;
            tail = sq->tail;
        }
        else
        {
            head = sq->head;
            tail = sq->tail;
        }
        size += sq->size;
    }
};