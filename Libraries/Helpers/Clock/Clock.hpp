#pragma once
#include <chrono>

namespace Chrono = std::chrono;

class Clock
{
public:
    Clock() = default;
    
    void Start();
    void End();
    [[nodiscard]] double GetDeltaTime() const;

private:
    Chrono::time_point<Chrono::high_resolution_clock> _startTime;
    Chrono::time_point<Chrono::high_resolution_clock> _endTime;
};

/**
 *  Sets start time of clock
 */
inline void Clock::Start()
{
    this->_startTime = Chrono::high_resolution_clock::now();
}

/**
 *  Sets end time of clock
 */
inline void Clock::End()
{
    this->_endTime = Chrono::high_resolution_clock::now();
}

/**
 *
 *  @return DeltaTime in seconds
 */

inline double Clock::GetDeltaTime() const
{
    const Chrono::duration<double> timeDiff = this->_endTime - this->_startTime;
    return timeDiff.count();
}
