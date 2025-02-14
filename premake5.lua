require "clean"
require "vscode"

workspace "HexEngine"
    location "Generated"
    cppdialect "C++23"
    configurations
    {
        "debug",
        "release"
    }

    architecture "x86_64"
    staticruntime "on"

    filter "configurations:debug"
        runtime "Debug"
        defines { "DEBUG" }
        symbols "On"
    filter "configurations:release"
        runtime "Release"
        defines { "NDEBUG" }
        optimize "On"

    targetBuildPath = path.getdirectory(_SCRIPT) .. "/Build/target"
    objBuildPath = path.getdirectory(_SCRIPT) .. "/Build/obj"

include "External"
include "Engine"
include "Game"
include "Test"