#include "Application.hpp"
#include "Setup.hpp"

#include <iostream>

Application::Application(const HINSTANCE& instance) :
    _window(Setup::SetupWindow(instance))
{
 
}

void Application::Run()
{
    Setup();

    // Render Loop
    double deltaTime = 0.0;
    while (!this->_exitHandler->ShouldTerminateProcess())
    {
        this->_clock.Start();

        RenderFrame();

        this->_clock.End();
        deltaTime = this->_clock.GetDeltaTime();
    }
}

void Application::Setup()
{
    ShowWindow(_window, SW_SHOWNORMAL);

    std::shared_ptr<IObserver> observerPtr = std::static_pointer_cast<IObserver>(this->_exitHandler);
    std::pair<InputAction, std::vector<std::shared_ptr<IObserver>>> pair = std::make_pair(Quit, std::vector<std::shared_ptr<IObserver>>({observerPtr}));
    std::unordered_map<InputAction, std::vector<std::shared_ptr<IObserver>>> map;
    map.insert(pair);
    this->_inputManager.SetInputObservers(map);
}

void Application::RenderFrame()
{
    
}

