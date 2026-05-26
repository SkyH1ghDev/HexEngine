#include <PCH/EnginePCH.hpp>
#include "SDLWindow.hpp"

using namespace HexEngine::SDL;

SDLWindow::SDLWindow(SDL_Window* pWindow, const std::uint32_t pWidth, const std::uint32_t pHeight)
{
    mWindow = std::shared_ptr<SDL_Window> {pWindow, SDL_DestroyWindow};
    mWidth = pWidth;
    mHeight = pHeight;
}