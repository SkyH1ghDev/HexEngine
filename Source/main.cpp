#define _CRTDBG_MAP_ALLOC

#include <crtdbg.h>

#include <Windows.h>
#include <d3d12.h>
#include "DirectXMath.h"

#include <iostream>

namespace DX = DirectX;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR    lpCmdLine,
                      _In_ int       nCmdShow)
{

	// Braces to let the application go out of scope before checking memory leaks
	{
		std::cout << "Test";
	}

	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	_CrtDumpMemoryLeaks();

	return 0;
}
