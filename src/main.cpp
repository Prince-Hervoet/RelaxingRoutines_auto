#include <iostream>
#include "simple_routine.hpp"
#include <thread>
#include <chrono>

typedef struct
{
    int a;
} Test;

void test(void *args)
{
    Test *t = (Test *)args;
    std::cout << t->a << std::endl;
    delete (Test *)t;
}

int main()
{
    for (int i = 0; i < 100; i++)
    {
        Test *t1 = new Test();
        t1->a = i;
        Test *t2 = new Test();
        t2->a = i;
        simple_start(test, t1);
        simple_start(test, t2);
        // std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    // Test *t = new Test();
    // t->a = 123;
    // std::cout << t << std::endl;

    // simple_start(test, nullptr);
    // std::this_thread::sleep_for(std::chrono::seconds(3));

    for (;;)
    {
    }
    return 0;
}