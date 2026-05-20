#pragma once
#include <unordered_map>
#include <functional>
#include <SDL3/SDL_scancode.h>

namespace HexEngine::Input
{
    class InputHandler 
    {
    public:
        InputHandler() = default;
        ~InputHandler() = default;
        InputHandler(const InputHandler& other) = default;
        InputHandler& operator=(const InputHandler& other) = default;
        InputHandler(InputHandler&& other) noexcept = default;
        InputHandler& operator=(InputHandler&& other) noexcept = default;

        static void Initialize();
        static void Update();
    
        static void UpdateKey(const SDL_Scancode& scancode, const bool& isPressed);
        static void BindKey(const SDL_Scancode& scancode, const std::function<void(const bool&)>& callbackFunc);
        static void UnbindKey(const SDL_Scancode& scancode, const std::function<void(const bool&)>& callbackFunc);
        static void ClearKey(const SDL_Scancode& scancode);
    
    private:
        inline static std::unordered_map<SDL_Scancode, std::vector<std::function<void(const bool&)>>> m_keyBinds {};
        inline static std::unordered_map<SDL_Scancode, bool> m_keyStates {};
    
    };
}
