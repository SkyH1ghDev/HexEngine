#pragma once

#include "Key_tmp.hpp"
#include <vector>
#include <memory>

class Keyboard_tmp
{
public:
    Keyboard_tmp() = default;
    Keyboard_tmp(const Keyboard_tmp& other);
    Keyboard_tmp& operator=(const Keyboard_tmp& other);
    Keyboard_tmp(const std::vector<std::shared_ptr<Key_tmp>>& keys);

public:
    std::vector<std::shared_ptr<Key_tmp>> GetPressedKeys();
    
private:
    std::vector<std::shared_ptr<Key_tmp>> _keys;
};
