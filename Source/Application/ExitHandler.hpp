#pragma once

#include "IObserver.hpp"

struct ExitData
{
    bool ShouldTerminate;
};

class ExitHandler : public IObserver
{
public:
    ExitHandler();
    ~ExitHandler() override = default;
    ExitHandler(const ExitHandler& other) = default;
    ExitHandler& operator=(const ExitHandler& other) = default;
    ExitHandler(ExitHandler&& other) noexcept = default;
    ExitHandler& operator=(ExitHandler&& other) noexcept = default;

    void Update(std::any data) override;

    [[nodiscard]] bool ShouldTerminateProcess() const;

private:

    bool _shouldTerminate = false;
};

inline bool ExitHandler::ShouldTerminateProcess() const
{
    return _shouldTerminate;
}
