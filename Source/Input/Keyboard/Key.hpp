#pragma once
#include "IObservable.hpp"

enum InputAction : std::uint8_t
{
    Unbound,
    Quit
};

class Key : public IObservable
{
public:
    Key() = delete;
    ~Key() override = default;
    Key(const Key& other) = default;
    Key& operator=(const Key& other) = default;
    Key(Key&& other) noexcept = default;
    Key& operator=(Key&& other) noexcept = default;

    void Attach(const std::shared_ptr<IObserver>& observer) override;
    void Detach(const std::shared_ptr<IObserver>& observer) override;
    void Notify() override;

    Key(const int& VirtualKey);
    Key(const int& VirtualKey, const InputAction& action);

private:
    int _virtualKey = 0;
    InputAction _action = Unbound;
};
