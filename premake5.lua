workspace "DirectX12"
    cppdialect "C++20"
    configurations {"Debug64"}
    filter "configurations:Debug64"
        defines {"_DEBUG", "_WINDOWS"}
        symbols "On"
        architecture "x86_64"
    project "DirectX12"
        kind "WindowedApp"
        language "C++"
        targetdir "Build"
        objdir "Binaries"
        includedirs {"Src/**"}
        files {"Src/**"}
        links {"d3d12", "d3dcompiler", "DXGI"}
        filter { "files:**.hlsl"}
            shadermodel("5.0")
            buildaction("none")
        filter {"files:vs**.hlsl"}
            shadertype "Vertex"
        filter {"files:ps**.hlsl"}
            shadertype "Pixel"
        filter {"files:hs**.hlsl"}
            shadertype "Hull"
        filter {"files:gs**.hlsl"}
            shadertype "Geometry"
        filter {"files:ds**.hlsl"}
            shadertype "Domain"
        filter {"files:cs**.hlsl"}


        --local projectPath = path.getabsolute(project().location or "./")
        --prebuildcommands {"python prebuild.py \"" .. projectPath .. "/\""}