#ifndef BUMP_CLOCK_H
#define BUMP_CLOCK_H

#include <chrono>
#include <future>

class Clock
{
public:
    Clock()
    {
        last = std::chrono::steady_clock::now();
        delta = 0.0;
        running = false;
    }
    void tick();
    float deltatime(bool precise = false);

    void run(float frequency, std::function<void()>);

    void stop();

private:
    std::chrono::time_point<std::chrono::steady_clock> last;
    float delta;
    bool running;
    std::future<void> clock_future;
};

extern Clock mainClock;

#endif //BUMP_CLOCK_H
