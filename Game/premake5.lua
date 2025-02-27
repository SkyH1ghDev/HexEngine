project "Game"

    location(projectsPath)
    
    kind "ConsoleApp"
    targetdir(targetBuildPath .. "/%{prj.name}")
    objdir(objBuildPath .. "/%{prj.name}")
    files {"src/**.h", "src/**.cpp"}
    includedirs{"../Engine/include", targetBuildPath .. "/External/include"}

    libdirs{targetBuildPath .. "/External/lib/"}

    dependson{"SDL3", "Engine"}

    links{"SDL3-static", "imagehlp", "setupapi", "user32", "version", "uuid", "winmm", "imm32", 
          "Engine",
          "DirectXTK12", 
          "DirectX-Headers",
          "DirectX-Guids",
          "d3d12",
          "DXGI"
    }
