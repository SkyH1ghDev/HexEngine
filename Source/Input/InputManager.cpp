#include "InputManager.hpp"

MSG InputManager::ReadMessage()
{
	MSG msg;

	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg;
}

void InputManager::HandleInput()
{
	MSG msg = ReadMessage();
}
