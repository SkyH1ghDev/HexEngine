#include "Key_tmp.hpp"

Key_tmp::Key_tmp(const int& virtualKey, const InputAction& action)
{
    this->_virtualKey = virtualKey;
    this->_action = action;
}

Key_tmp::Key_tmp(const int& virtualKey)
{
    this->_virtualKey = virtualKey;
    this->_action = Unbound;
}

bool Key_tmp::IsHeldDown()
{
    return this->_isHeldDown;
}

void Key_tmp::SetHoldDownState(const bool& val)
{
    this->_isHeldDown = val;
}

bool Key_tmp::IsPressed()
{
    return this->_isPressed;
}

void Key_tmp::SetPressedState(const bool& val)
{
    this->_isPressed = val;
}

int Key_tmp::GetVirtualKey()
{
    return this->_virtualKey;
}

InputAction Key_tmp::GetAction()
{
    return this->_action;
}




