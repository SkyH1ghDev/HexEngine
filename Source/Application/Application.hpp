#pragma once

#include <Windows.h>

#include "Clock.hpp"
#include "ExitHandler.hpp"
#include "InputManager.hpp"

class Application
{
public:
    Application(const HINSTANCE& instance);
    
public:
    void Run();

private:
    void Setup();
    void RenderFrame();

private:
    HWND _window;
    InputManager _inputManager;
    Clock _clock;

    std::shared_ptr<ExitHandler> _exitHandler = std::make_shared<ExitHandler>();
};
