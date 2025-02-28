#include "Clock.hpp"
#include <iostream>

std::uint64_t Clock::frameCount = 0;
double Clock::elapsedSeconds = 0.0;
std::chrono::high_resolution_clock::time_point Clock::t0 = std::chrono::high_resolution_clock::now();
std::chrono::high_resolution_clock::time_point Clock::t1 = {};
std::chrono::high_resolution_clock::duration Clock::deltaTime = {};

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
