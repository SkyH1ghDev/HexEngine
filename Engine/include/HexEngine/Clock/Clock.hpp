#pragma once

class Clock
{
public:
    Clock() = default;
    ~Clock() = default;
    Clock(const Clock& other) = default;
    Clock& operator=(const Clock& other) = default;
    Clock(Clock&& other) noexcept = default;
    Clock& operator=(Clock&& other) noexcept = default;
};
