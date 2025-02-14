#pragma once

class EngineLoop
{
public:
    EngineLoop() = default;
    ~EngineLoop() = default;
    EngineLoop(const EngineLoop& other) = default;
    EngineLoop& operator=(const EngineLoop& other) = default;
    EngineLoop(EngineLoop&& other) noexcept = default;
    EngineLoop& operator=(EngineLoop&& other) noexcept = default;

};
