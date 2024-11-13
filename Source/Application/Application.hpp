#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

class Application
{
public:
    Application(const HINSTANCE& instance);
    
public:
    /**
     * Run the engine
     */
    void Run();

private:
    /**
     * Setup the engine 
     */
    void Setup();

    /**
     * Being render loop
     */
    void Render();

private:
    HWND _window;
};
