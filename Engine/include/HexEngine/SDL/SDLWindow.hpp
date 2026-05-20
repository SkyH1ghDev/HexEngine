#pragma once
#include <SDL3/SDL_video.h>
#include <memory>

namespace HexEngine::SDL
{
    class SDLWindow
    {
    public:
        SDLWindow() = default;
        SDLWindow(SDL_Window* window);
        ~SDLWindow() = default;
        SDLWindow(const SDLWindow& other) = delete;
        SDLWindow& operator=(const SDLWindow& other) = delete;
        SDLWindow(SDLWindow&& other) noexcept = default;
        SDLWindow& operator=(SDLWindow&& other) noexcept = default;

        std::weak_ptr<SDL_Window> GetSDLWindow() const;
    
    private:
        std::shared_ptr<SDL_Window> m_window {nullptr, SDL_DestroyWindow};
    };
    
    inline std::weak_ptr<SDL_Window> SDLWindow::GetSDLWindow() const
    {
        return m_window;
    }
}

