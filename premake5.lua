workspace "HexEngine"
    cppdialect "C++20"
    configurations {"Debug64", "Release64", "Debug32", "Release32"}
    filter "configurations:Debug64"
        defines {"_DEBUG", "_WINDOWS"}
        symbols "Full"
        architecture "x86_64"
        
    filter "configurations:Debug32"
        defines {"_DEBUG", "_WINDOWS"}
        symbols "Full"
        architecture "x86"
        
    filter "configurations:Release64"
        defines {"_WINDOWS"}
        symbols "Off"
        architecture "x86_64"
        optimize "Full"
        
    filter "configurations:Release32"
        defines {"_WINDOWS"}
        symbols "Off"
        architecture "x86"
        optimize "Full"
        
    project "HexEngine"
        kind "WindowedApp"
        language "C++"
        targetdir "Build"
        objdir "Binaries"
        includedirs {"Source/**"}
        files {"Source/**"}
        links {"d3d12", "d3dcompiler", "DXGI"}
        filter {"files:**.hlsl"}
            shadermodel("6.5")
            buildaction("FxCompile")
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
           shadertype "Compute"
        filter {"files:ms**.hlsl"}
            shadertype "Mesh"
        filter {"files:as**.hlsl"}
            shadertype "Amplification"

        --local projectPath = path.getabsolute(project().location or "./")
        --prebuildcommands {"python prebuild.py \"" .. projectPath .. "/\""}