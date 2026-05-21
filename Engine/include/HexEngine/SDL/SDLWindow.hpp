#pragma once
#include <SDL3/SDL_video.h>
#include <memory>

namespace HexEngine::SDL
{
    class SDLWindow
    {
    public:
        SDLWindow() = default;
        explicit SDLWindow(SDL_Window* pWindow, const std::uint32_t pWidth = 1280, const std::uint32_t pHeight = 720);
        ~SDLWindow() = default;
        SDLWindow(const SDLWindow& pOther) = delete;
        SDLWindow& operator=(const SDLWindow& pOther) = delete;
        SDLWindow(SDLWindow&& pOther) noexcept = default;
        SDLWindow& operator=(SDLWindow&& pOther) noexcept = default;

        std::weak_ptr<SDL_Window> GetSDLWindow() const;
        std::uint32_t GetWidth() const;
        std::uint32_t GetHeight() const;
        
    private:
        std::shared_ptr<SDL_Window> mWindow {nullptr, SDL_DestroyWindow};
        std::uint32_t mWidth {1280};
        std::uint32_t mHeight {720};
    };
    
    inline std::weak_ptr<SDL_Window> SDLWindow::GetSDLWindow() const
    {
        return mWindow;
    }
    
    inline std::uint32_t SDLWindow::GetWidth() const
    {
        return mWidth;
    }
    
    inline std::uint32_t SDLWindow::GetHeight() const
    {
        return mHeight;
    }
}

