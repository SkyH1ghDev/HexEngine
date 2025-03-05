#include "Input.hpp"

std::unordered_map<SDL_Scancode, std::function<void()>> Input::m_keyBinds = {};
std::unordered_map<SDL_Scancode, bool> Input::m_keyStates = {};

void Input::Initialize()
{
    for (int i = SDL_SCANCODE_A; i <= SDL_SCANCODE_NONUSBACKSLASH; ++i)
    {
        m_keyBinds[static_cast<SDL_Scancode>(i)] = std::function<void()>{};
        m_keyStates[static_cast<SDL_Scancode>(i)] = false;
    }
}

void Input::Update()
{
    for (auto it = m_keyBinds.begin(); it != m_keyBinds.end(); ++it)
    {
        if (m_keyStates.at(it->first) == true)
        {
            std::function<void()> action = it->second;
            action();
        }
    }   
}

void Input::UpdateKey(const SDL_Scancode& scancode, const bool& isPressed)
{
    m_keyStates.at(scancode) = isPressed;
}
