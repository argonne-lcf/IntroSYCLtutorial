#include "Timer.hpp"

void Timer::start()
{
    t_start = std::chrono::high_resolution_clock::now();
}

void Timer::stop()
{
    t_end = std::chrono::high_resolution_clock::now();
}

float Timer::getDuration()
{
    std::chrono::microseconds time_elapsed = std::chrono::duration_cast<std::chrono::microseconds>(t_end - t_start);
    return time_elapsed.count();
}
