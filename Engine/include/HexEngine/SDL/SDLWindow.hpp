#pragma once
#include <SDL3/SDL_video.h>

class SDLWindow
{
public:
    SDLWindow() = default;
    SDLWindow(SDL_Window* window);
    ~SDLWindow();
    SDLWindow(const SDLWindow& other) = default;
    SDLWindow& operator=(const SDLWindow& other) = default;
    SDLWindow(SDLWindow&& other) noexcept = default;
    SDLWindow& operator=(SDLWindow&& other) noexcept = default;

    SDL_Window* GetSDLWindow() const;
    
private:
    SDL_Window* m_window = nullptr;
    
};

inline SDL_Window* SDLWindow::GetSDLWindow() const
{
    return m_window;
}