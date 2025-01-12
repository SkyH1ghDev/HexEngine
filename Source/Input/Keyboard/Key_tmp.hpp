#pragma once

enum InputAction
{
    Unbound,
    Quit,
    
    MoveUp,
    MoveDown,
    MoveForward,
    MoveBackward,
    MoveLeft,
    MoveRight,
    
    ToggleOutputMode,
    ToggleMouseLock,
    ToggleCamera
};

class Key_tmp
{
public:
    Key_tmp(const int& virtualKey, const InputAction& action);
    Key_tmp(const int& virtualKey);

public:
    bool IsHeldDown();
    void SetHoldDownState(const bool& val);

    bool IsPressed();
    void SetPressedState(const bool& val);
    
    int GetVirtualKey();
    InputAction GetAction();
    
private:
    int _virtualKey;
    InputAction _action;

    bool _isHeldDown = false;
    bool _isPressed = false;
};
