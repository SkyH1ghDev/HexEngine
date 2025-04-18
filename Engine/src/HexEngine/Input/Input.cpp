#include "Input.hpp"

std::unordered_map<SDL_Scancode, std::vector<std::function<void(const bool&)>>> Input::m_keyBinds = {};
std::unordered_map<SDL_Scancode, bool> Input::m_keyStates = {};

void Input::Initialize()
{
    for (int i = SDL_SCANCODE_A; i <= SDL_SCANCODE_NONUSBACKSLASH; ++i)
    {
        m_keyBinds[static_cast<SDL_Scancode>(i)] = { };
        m_keyStates[static_cast<SDL_Scancode>(i)] = false;
    }
}

void Input::Update()
{
    for (const auto& keyBind : m_keyBinds)
    {
        for (const auto& action : keyBind.second)
        {
            action(m_keyStates[static_cast<SDL_Scancode>(keyBind.first)]);
        }
    }   
}

void Input::UpdateKey(const SDL_Scancode& scancode, const bool& isPressed)
{
    m_keyStates.at(scancode) = isPressed;
}

void Input::BindKey(const SDL_Scancode& scancode, const std::function<void(const bool&)>& callbackFunc)
{
    bool functionFound = 
    std::ranges::find_if
    (
        m_keyBinds.at(scancode),
        [&callbackFunc](const std::function<void(const bool&)>& action)
        {
            return action.target_type() == callbackFunc.target_type();
        }
    )
    != m_keyBinds.at(scancode).end();
    
    if (!functionFound)
    {
        m_keyBinds[scancode].push_back(callbackFunc);
    }
}

void Input::UnbindKey(const SDL_Scancode& scancode, const std::function<void(const bool&)>& callbackFunc)
{
    std::ranges::borrowed_iterator_t<std::vector<std::function<void(const bool&)>>&> keybindIterator =  
    std::ranges::find_if
    (
        m_keyBinds.at(scancode),
        [&callbackFunc](const std::function<void(const bool&)>& action)
        {
            return action.target_type() == callbackFunc.target_type();
        }
    );

    bool functionFound = m_keyBinds.at(scancode).end() != keybindIterator;
    
    if (functionFound)
    {
        m_keyBinds.at(scancode).erase(keybindIterator);
    }
}

void Input::ClearKey(const SDL_Scancode& scancode)
{
    m_keyBinds.at(scancode).clear();
}
