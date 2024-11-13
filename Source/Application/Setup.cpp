#include "Setup.hpp"
#include <stdexcept>

/*
 * PUBLIC
 */

HWND Setup::SetupWindow(const HINSTANCE& instance)
{
    constexpr wchar_t CLASS_NAME[] = L"DirectX12 3D Window";

    WNDCLASS wc;
	ZeroMemory(&wc, sizeof(wc));
	
	wc.lpszClassName = CLASS_NAME;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = instance;

	if (!RegisterClass(&wc))
	{
		throw std::runtime_error("Failed To Register Window Class");
	}

	constexpr wchar_t WINDOW_NAME[] = L"HexEngine Alpha v0.1";
	constexpr int WIDTH = 1280;
	constexpr int HEIGHT = 720;
	HWND window = CreateWindowEx
	(
		0,
		CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		WIDTH,
		HEIGHT,
		nullptr,
		nullptr,
		instance,
		nullptr
	);

	if (window == nullptr)
	{
		throw std::runtime_error("Failed to Create Window");
	}
	
	return window;
}

/*
 * PRIVATE
 */

LRESULT CALLBACK Setup::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// sort through and find what code to run for the message given
	switch (message)
	{
		// this message is read when the window is closed
		case WM_DESTROY:
		    // close the application entirely
			PostQuitMessage(0);
			return 0;
		
		default:
			break;
	}

	// Handle any messages the switch statement didn't by using default methodology
	return DefWindowProc(hWnd, message, wParam, lParam);
}
