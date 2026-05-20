#include "SDLWindow.hpp"

using namespace HexEngine::SDL;

SDLWindow::SDLWindow(SDL_Window* window)
{
    m_window = std::shared_ptr<SDL_Window> {window, SDL_DestroyWindow};
}