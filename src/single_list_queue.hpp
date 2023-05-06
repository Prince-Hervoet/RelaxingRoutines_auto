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
        // std::unique_lock<std::mutex> lock(mu);
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

    T *getFirst()
    {
        if (size == 0)
        {
            return nullptr
        }
        // std::unique_lock<std::mutex> lock(mu);
        ListNode *temp = head->next;
        ListNode *target = head;
        head->next = nullptr;
        head = temp;
        size -= 1;
        return target;
    }

    void joinOtherList(SingleListQueue *other)
    {
        }
};