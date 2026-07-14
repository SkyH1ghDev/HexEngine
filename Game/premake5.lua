project "Game"

    location(projectsPath)
    
    kind "ConsoleApp"
    targetdir(targetBuildPath .. "/%{prj.name}")
    objdir(objBuildPath .. "/%{prj.name}")
    files {"src/**.h", "src/**.cpp"}
    includedirs{"../Engine/include", targetBuildPath .. "/External/include"}

    libdirs{targetBuildPath .. "/External/lib/"}

    dependson {"Engine"}

    links {
        AddQuotation("SDL3"),
        "Engine",
        "vulkan",
        "ImGui"
    }

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


