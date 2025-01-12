#pragma once
#include <windows.h>
#include <unordered_map>

#include "IObservable.hpp"
#include "Keyboard.hpp"

class InputManager
{
public:
    InputManager() = default;
    ~InputManager() = default;
    InputManager(const InputManager& other) = default;
    InputManager& operator=(const InputManager& other) = default;
    InputManager(InputManager&& other) noexcept = default;
    InputManager& operator=(InputManager&& other) noexcept = default;

    void HandleInput();
    void SetInputObservers(const std::unordered_map<InputAction, std::vector<std::shared_ptr<IObserver>>>& inputObservers);

private:
    [[nodiscard]] MSG ReadMessage();

private:
    Keyboard _keyboard;
    std::unordered_map<InputAction, std::vector<std::shared_ptr<IObserver>>> _inputObservers;
};

inline void InputManager::SetInputObservers(const std::unordered_map<InputAction, std::vector<std::shared_ptr<IObserver>>>& inputObservers)
{
	this->_inputObservers = inputObservers;

    // TODO: WRITE LOGIC THAT MAKES THE KEYS LINK TO THE OBSERVERS
}
