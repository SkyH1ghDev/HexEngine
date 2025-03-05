#include "SDLEventHandler.hpp"

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>

#include "EngineCore.hpp"
#include "Input.hpp"

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
                EngineCore::Quit();
                break;
                
            default:
                break;
        }
    }
}
