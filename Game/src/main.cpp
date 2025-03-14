#define _CRTDBG_MAP_ALLOC

#include <crtdbg.h>
#include <HexEngine/Engine/EngineCore.hpp>

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    
    EngineCore::Run();

    return 0;
}
