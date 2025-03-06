#include "SDLEventHandler.hpp"
#include <HexEngine/Input/Input.hpp>
#include <HexEngine/Engine/EngineLoop.hpp>

#include <SDL3/SDL_events.h>


void SDLEventHandler::HandleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_EVENT_KEY_DOWN:
                Input::UpdateKey(event.key.scancode, true);
                break;

            case SDL_EVENT_KEY_UP:
                Input::UpdateKey(event.key.scancode, false);
                break;

            case SDL_EVENT_QUIT:
                EngineLoop::Stop();
                break;
                
            default:
                break;
        }
    }
}
