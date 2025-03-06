#pragma once
#include <unordered_map>
#include <functional>
#include <SDL3/SDL_scancode.h>

class Input {
public:
    Input() = default;
    ~Input() = default;
    Input(const Input& other) = default;
    Input& operator=(const Input& other) = default;
    Input(Input&& other) noexcept = default;
    Input& operator=(Input&& other) noexcept = default;

    static void Initialize();
    static void Update();
    
    static void UpdateKey(const SDL_Scancode& scancode, const bool& isPressed);
    static void BindKey(const SDL_Scancode& scancode, const std::function<void()>& callbackFunc);
    static void UnbindKey(const SDL_Scancode& scancode, const std::function<void()>& callbackFunc);
    
private:
    static std::unordered_map<SDL_Scancode, std::vector<std::function<void()>>> m_keyBinds;
    static std::unordered_map<SDL_Scancode, bool> m_keyStates;
    
};
