#include <iostream>
#include "simple_routine.hpp"
#include <thread>
#include <chrono>

void test(void *args)
{
    std::cout << "asdf" << std::endl;
}

int main()
{
    simple_start(test, nullptr);
    std::this_thread::sleep_for(std::chrono::seconds(3));
    simple_start(test, nullptr);
    std::this_thread::sleep_for(std::chrono::seconds(3));

    for (;;)
    {
    }
    return 0;
}