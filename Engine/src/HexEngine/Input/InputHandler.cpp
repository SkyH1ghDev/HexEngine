#include <PCH/EnginePCH.hpp>
#include "InputHandler.hpp"

using namespace HexEngine::Input;

void InputHandler::Initialize()
{
    for (int i = SDL_SCANCODE_A; i <= SDL_SCANCODE_NONUSBACKSLASH; ++i)
    {
        m_keyBinds[static_cast<SDL_Scancode>(i)] = { };
        m_keyStates[static_cast<SDL_Scancode>(i)] = false;
    }
}

void InputHandler::Update()
{
    for (const auto& keyBind : m_keyBinds)
    {
        for (const auto& action : keyBind.second)
        {
            action(m_keyStates[static_cast<SDL_Scancode>(keyBind.first)]);
        }
    }   
}

void InputHandler::UpdateKey(const SDL_Scancode& scancode, const bool& isPressed)
{
    m_keyStates.at(scancode) = isPressed;
}

void InputHandler::BindKey(const SDL_Scancode& scancode, const std::function<void(const bool&)>& callbackFunc)
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

void InputHandler::UnbindKey(const SDL_Scancode& scancode, const std::function<void(const bool&)>& callbackFunc)
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

    if (m_keyBinds.at(scancode).end() != keybindIterator)
    {
        m_keyBinds.at(scancode).erase(keybindIterator);
    }
}

void InputHandler::ClearKey(const SDL_Scancode& scancode)
{
    m_keyBinds.at(scancode).clear();
}
