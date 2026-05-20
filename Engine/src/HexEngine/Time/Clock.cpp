#include "Clock.hpp"
#include <iostream>

using namespace HexEngine::Time;

double Clock::GetDeltaTime()
{
    return static_cast<double>(deltaTime.count());
}

void Clock::Update()
{
    ++frameCount;
    t1 = std::chrono::high_resolution_clock::now();
    deltaTime = t1 - t0;
    t0 = t1;

    elapsedSeconds += GetDeltaTime() * 1e-9;

    if (elapsedSeconds > 1.0)
    {
        double fps = static_cast<double>(frameCount) / elapsedSeconds;
        std::cout << std::format("FPS = {:.2f}\n", fps) << std::flush;

        frameCount = 0;
        elapsedSeconds = 0.0;
    }
}
