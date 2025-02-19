#pragma once

#include <SDL3/SDL.h>
#include <memory>

template<>
struct std::default_delete<SDL_Window>
{
    void operator()(SDL_Window* ptr) const
    {
        SDL_DestroyWindow(ptr);
    }
};
