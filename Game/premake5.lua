project "Game"

    location(projectsPath)
    
    kind "ConsoleApp"
    targetdir(targetBuildPath .. "/%{prj.name}")
    objdir(objBuildPath .. "/%{prj.name}")
    files {"src/**.h", "src/**.cpp"}
    includedirs{"../Engine/include", targetBuildPath .. "/External/include"}

    libdirs{targetBuildPath .. "/External/lib/"}

    dependson{"Engine"}

    filter "system:windows"
        links {
            "image",
            "imagehlp",
            "setupapi",
            "user32",
            "version",
            "uuid",
            "winmm",
            "imm32"
        }

    links {
        "vulkan",
        "SDL3-static",
        "Engine",
        "ImGui"
    }
