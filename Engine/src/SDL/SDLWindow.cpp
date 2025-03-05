#include "SDLWindow.hpp"

SDLWindow::SDLWindow(SDL_Window* window)
{
    m_window = window;
}

SDLWindow::~SDLWindow()
{
    SDL_DestroyWindow(m_window);
}
