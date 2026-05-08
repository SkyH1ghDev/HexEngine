#include "Clock.hpp"
#include <iostream>

std::uint64_t Clock::m_frameCount = 0;
double Clock::m_elapsedSeconds = 0.0;
std::chrono::high_resolution_clock::time_point Clock::m_t0 = std::chrono::high_resolution_clock::now();
std::chrono::high_resolution_clock::time_point Clock::m_t1 = {};
std::chrono::high_resolution_clock::duration Clock::m_deltaTime = {};

double Clock::GetDeltaTime()
{
    return static_cast<double>(m_deltaTime.count());
}

void Clock::Update()
{
    ++m_frameCount;
    m_t1 = std::chrono::high_resolution_clock::now();
    m_deltaTime = m_t1 - m_t0;
    m_t0 = m_t1;

    m_elapsedSeconds += GetDeltaTime() * 1e-9;

    if (m_elapsedSeconds > 1.0)
    {
        double fps = static_cast<double>(m_frameCount) / m_elapsedSeconds;
        std::cout << std::format("FPS = {:.2f}\n", fps) << std::flush;

        m_frameCount = 0;
        m_elapsedSeconds = 0.0;
    }
}
