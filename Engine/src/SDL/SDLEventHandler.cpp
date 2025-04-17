#include "SDLEventHandler.hpp"
#include <HexEngine/Input/Input.hpp>
#include <HexEngine/Engine/EngineLoop.hpp>

#include <SDL3/SDL_events.h>
#include <ImGui/imgui_impl_sdl3.h>

void SDLEventHandler::HandleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        #if defined(_DEBUG)
            ImGui_ImplSDL3_ProcessEvent(&event);
        #endif
        
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
