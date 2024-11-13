#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

class Setup
{
public:
    /**
     * Sets up basic Windows window
     * @param instance Application instance handle
     * @return Window 
     */
    static HWND SetupWindow(const HINSTANCE& instance);
    
private:
    /**
     * Window message handler. To be assigned to the window at creation.
     * @param hWnd Window handle
     * @param message Window message
     * @param wParam Additional message information
     * @param lParam Additional message information
     * @return Result of the message handling
     */
    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};
