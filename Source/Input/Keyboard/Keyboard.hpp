#pragma once

#include <vector>

#include "Key.hpp"

class Keyboard
{
public:
    Keyboard();
    ~Keyboard() = default;
    Keyboard(const Keyboard& other) = default;
    Keyboard& operator=(const Keyboard& other) = default;
    Keyboard(Keyboard&& other) noexcept = default;
    Keyboard& operator=(Keyboard&& other) noexcept = default;

private:
    std::vector<Key> _keys;

};
