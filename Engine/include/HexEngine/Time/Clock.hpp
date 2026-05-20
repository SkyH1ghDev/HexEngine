#pragma once
#include <cstdint>
#include <chrono>

namespace HexEngine::Time
{
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
        inline static std::uint64_t frameCount {};
        inline static double elapsedSeconds {};
        inline static std::chrono::high_resolution_clock::time_point t0 {std::chrono::high_resolution_clock::now()};
        inline static std::chrono::high_resolution_clock::time_point t1 {};
        inline static std::chrono::high_resolution_clock::duration deltaTime {};
    };
}
