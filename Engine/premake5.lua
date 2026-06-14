project "Engine"
    kind "StaticLib"

    location(projectsPath)

    targetdir(targetBuildPath .. "/%{prj.name}")
    objdir(objBuildPath .. "/%{prj.name}")
    includedirs {"include/", "include/**",  targetBuildPath .. "/External/include/"}
    libdirs {targetBuildPath .. "/External/lib/"}
    dependson{"SDL3", "ImGui"}
    buildoptions { "-FIEnginePCH.hpp" }

    files {
        "include/PCH/EnginePCH.hpp",
        "src/PCH/EnginePCH.cpp",
        "include/**.hpp", 
        "src/**.cpp", 
        "src/**.slang"
    }

    pchheader "EnginePCH.hpp"
    pchsource "src/PCH/EnginePCH.cpp"