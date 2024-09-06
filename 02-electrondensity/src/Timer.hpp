#ifndef _TIMER_HPP_
#define _TIMER_HPP_

#include <chrono>

class Timer
{
private:
    std::chrono::high_resolution_clock::time_point t_start;
    std::chrono::high_resolution_clock::time_point t_end;

public:
    void start();
    void stop();
    float getDuration();
};

#endif
