#include <iostream>
#include "simple_routine.hpp"
#include <thread>
#include <chrono>
#include <signal.h>
#include <atomic>
std::atomic<uint64_t> finished{0};
typedef struct
{
    int a;
} Test;

void test1(void *args)
{
    Test *t = (Test *)args;
    // std::cout << t->a << std::endl;
    //  pthread_kill(pthread_self(), SIGUSR2);
    std::cout << "send sig to id:" << pthread_self() << " A" << std::endl;
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
    int i = 0;
    while (i < 1000)
    {
        std::cout << "i am id: " << pthread_self() << i++ << std::endl;
    }
    std::cout << "i am id: " << pthread_self() << "finished!" << std::endl;
    finished++;
}

int main()
{
    for (int i = 0; i < 1000; i++)
    {
        simple_start(test3, nullptr);
    }
    std::this_thread::sleep_for(std::chrono::seconds(20));
    std::cout << finished << std::endl;

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