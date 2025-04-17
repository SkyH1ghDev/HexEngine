require "clean"
require "vscode"

workspace "HexEngine"
    
    location "./"
    cppdialect "C++23"
    warnings "Extra"
    fatalwarnings { "All" }
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
        optimize "Off"
    filter "configurations:release"
        runtime "Release"
        defines { "NDEBUG" }
        optimize "On"

    rootPath = path.getdirectory(_SCRIPT)
    targetBuildPath = path.getdirectory(_SCRIPT) .. "/Build/target"
    objBuildPath = path.getdirectory(_SCRIPT) .. "/Build/obj"
    projectsPath = path.getdirectory(_SCRIPT) .. "/Generated/projects"

include "External"
include "Engine"
include "Game"
include "Test"