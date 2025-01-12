#define KEY_PRESSED 0x8000

#include "Keyboard_tmp.hpp"
#include <windows.h>
#include <iostream>

Keyboard_tmp::Keyboard_tmp(const Keyboard_tmp& other)
{
    this->_keys = other._keys;
}

Keyboard_tmp& Keyboard_tmp::operator=(const Keyboard_tmp& other)
{
    if (this == &other)
    {
        return *this;
    }

    this->_keys = other._keys;
    return *this;
}

Keyboard_tmp::Keyboard_tmp(const std::vector<std::shared_ptr<Key_tmp>>& keys)
{
    this->_keys = keys;
}

std::vector<std::shared_ptr<Key_tmp>> Keyboard_tmp::GetPressedKeys()
{
    std::vector<std::shared_ptr<Key_tmp>> pressedKeys;

    for (std::shared_ptr<Key_tmp>& key : this->_keys)
    {

        key->SetPressedState(GetAsyncKeyState(key->GetVirtualKey()) & KEY_PRESSED);
        
        if (key->IsPressed() || key->IsHeldDown())
        {
            pressedKeys.push_back(key);
        }
    }

    return pressedKeys;
}

