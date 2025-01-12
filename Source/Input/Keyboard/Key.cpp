#include "Key.hpp"

void Key::Attach(const std::shared_ptr<IObserver>& observer)
{

}

void Key::Detach(const std::shared_ptr<IObserver>& observer)
{

}

void Key::Notify()
{

}

Key::Key(const int& VirtualKey)
{
    this->_virtualKey = VirtualKey;
}

Key::Key(const int& VirtualKey, const InputAction& action)
{
    this->_virtualKey = VirtualKey;
    this->_action = action;
}
