#define _CRTDBG_MAP_ALLOC

#include <cstdint>
#include <crtdbg.h>
#include <HexEngine/Engine/EngineCore.hpp>

// Indicates to hybrid graphics systems to prefer the discrete part by default
extern "C" {
__declspec(dllexport) std::uint32_t NvOptimusEnablement {0x00000001};
__declspec(dllexport) std::int32_t AmdPowerXpressRequestHighPerformance {1};
}

int main([[maybe_unused]] std::int32_t argc, [[maybe_unused]] char* argv[])
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    
    HexEngine::Engine::EngineCore::Run();

    return 0;
}
