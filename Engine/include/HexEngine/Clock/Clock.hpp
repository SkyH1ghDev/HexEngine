#pragma once
#include <cstdint>
#include <chrono>

class Clock
{
public:
    Clock() = default;
    ~Clock() = default;
    Clock(const Clock& other) = default;
    Clock& operator=(const Clock& other) = default;
    Clock(Clock&& other) noexcept = default;
    Clock& operator=(Clock&& other) noexcept = default;

    static void Update();
    static double GetDeltaTime();
    
private:
    static std::uint64_t frameCount;
    static double elapsedSeconds;
    static std::chrono::high_resolution_clock::time_point t0;
    static std::chrono::high_resolution_clock::time_point t1;
    static std::chrono::high_resolution_clock::duration deltaTime;
};
