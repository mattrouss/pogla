#include <future>
#include <iostream>
#include "clock.h"

Clock mainClock;

void Clock::tick()
{
    auto now = std::chrono::steady_clock::now();
    auto d = std::chrono::duration<float>(now - last);

    delta = (float)d.count();
    last = now;
}

float Clock::deltatime(bool precise)
{
    if (!precise)
        return delta;
    else
    {
        auto now = std::chrono::steady_clock::now();
        auto d = std::chrono::duration<float>(now - last);
        return d.count();
    }
}

void Clock::run(float frequency, std::function<void()> callback)
{
    if (running)
        return;
    running = true;
    float freq = frequency;
    clock_future = std::async(std::launch::async, [freq, this, callback](){
        while (this->running)
        {
            auto start = std::chrono::steady_clock::now();
            auto delta = std::chrono::duration<float>(1.0/freq);
            std::this_thread::sleep_for(delta);
            auto end = std::chrono::steady_clock::now();
            std::chrono::duration<double, std::milli> elapsed = end -start;
            callback();
            this->tick();
        }
    });
}

void Clock::stop()
{
    if (!running)
        return;
    running = false;
    clock_future.wait();
}
