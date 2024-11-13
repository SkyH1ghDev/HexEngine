#include "Application.hpp"

#include "Setup.hpp"

#include <iostream>

/*
 * PUBLIC
 */

Application::Application(const HINSTANCE& instance) :
    _window(Setup::SetupWindow(instance))
{
 
}

void Application::Run()
{
    Setup();
    Render();
}

/*
 * PRIVATE
 */

void Application::Setup()
{
    ShowWindow(_window, SW_SHOWNORMAL);
}

void Application::Render()
{
    
}

