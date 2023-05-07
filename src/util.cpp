#include "util.hpp"
#include <chrono>

uint64_t getNowTimestamp()
{
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(duration);
    return static_cast<uint64_t>(microseconds.count());
}