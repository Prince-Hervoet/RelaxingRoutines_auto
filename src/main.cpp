#include <iostream>
#include "simple_routine.hpp"
#include <thread>
#include <chrono>
#include <signal.h>

typedef struct
{
    int a;
} Test;

void test1(void *args)
{
    Test *t = (Test *)args;
    //std::cout << t->a << std::endl;
    // pthread_kill(pthread_self(), SIGUSR2);
    std::cout << "send sig to id:" << pthread_self()<< " A" << std::endl;
    int i = 0;
    while (1)
    {
        /* code */
        // if (!(i++ % 40000))
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "i am A :" << i++ << std::endl;
    }
    delete (Test *)t;
}

void test2(void *args)
{
    Test *t = (Test *)args;
    // std::cout << t->a << std::endl;
    // pthread_kill(pthread_self(), SIGUSR2);
    std::cout << "send sig to id:" << pthread_self() << " B" << std::endl;
    int i = 0;
    while (1)
    {
        /* code */
        // if (!(i++ % 40000))
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "i am B :" << i++ << std::endl;
    }
    delete (Test *)t;
}

void test3(void *args)
{
    Test *t = (Test *)args;
    // std::cout << t->a << std::endl;
    //  pthread_kill(pthread_self(), SIGUSR2);
    std::cout << "send sig to id:" << pthread_self() << " C" << std::endl;
    int i = 0;
    while (1)
    {
        /* code */
        // if (!(i++ % 40000))
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "i am C :" << i++ << std::endl;
    }
    delete (Test *)t;
}

int main()
{
    // for (int i = 0; i < 100; i++)
    // {
    //     Test *t1 = new Test();
    //     t1->a = i;
    //     Test *t2 = new Test();
    //     t2->a = i;
    //     simple_start(test, t1);
    //     simple_start(test, t2);
    //     std::this_thread::sleep_for(std::chrono::seconds(1));
    // }
    Test *t1 = new Test();
    t1->a = 1;
    Test *t2 = new Test();
    t2->a = 2;
    Test *t3 = new Test();
    t2->a = 3;
    simple_start(test1, t1);
    simple_start(test2, t2);
    simple_start(test3, t3);
    std::this_thread::sleep_for(std::chrono::seconds(30));
    // Test *t = new Test();
    // t->a = 123;
    // std::cout << t << std::endl;

    //  simple_start(test, nullptr);
    //  std::this_thread::sleep_for(std::chrono::seconds(3));

    for (;;)
    {
    }
    return 0;
}