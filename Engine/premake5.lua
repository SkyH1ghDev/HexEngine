project "Engine"
    kind "StaticLib"

    location(projectsPath)

    targetdir(targetBuildPath .. "/%{prj.name}")
    objdir(objBuildPath .. "/%{prj.name}")
    includedirs {"include/", "include/**",  targetBuildPath .. "/External/include/"}
    libdirs {targetBuildPath .. "/External/lib/"}
    dependson{"SDL3", "DirectXToolKit", "DirectXHeaders", "ImGui"}
    buildoptions { "/FIEnginePCH.hpp" }

    files {
        "include/PCH/EnginePCH.hpp",
        "src/PCH/EnginePCH.cpp",
        "include/**.hpp", 
        "src/**.cpp", 
        "src/**.hlsl"
    }

    pchheader "EnginePCH.hpp"
    pchsource "src/PCH/EnginePCH.cpp"

    -- SHADER --
    shaderassembler("AssemblyCode")
    filter("files:**.hlsl")
        flags("ExcludeFromBuild")
        shaderobjectfileoutput(targetBuildPath .. "/Shader/cso/%%(Filename).cso")
        shaderassembleroutput(targetBuildPath .. "/Shader/asm/%%(Filename).asm")
        shadermodel("6.6")

    filter "files:**_vs.hlsl"
        removeflags("ExcludeFromBuild")
        shaderentry("main")
        shadertype("Vertex")
    
    filter "files:**_hs.hlsl"
        removeflags("ExcludeFromBuild")
        shaderentry("main")
        shadertype("Hull")
    
    filter "files:**_ds.hlsl"
        removeflags("ExcludeFromBuild")
        shaderentry("main")
        shadertype("Domain")
    
    filter "files:**_gs.hlsl"
        removeflags("ExcludeFromBuild")
        shaderentry("main")
        shadertype("Geometry")
    
    filter "files:**_ps.hlsl"
        removeflags("ExcludeFromBuild")
        shaderentry("main")
        shadertype("Pixel")
    
    filter "files:**_ms.hlsl"
        removeflags("ExcludeFromBuild")
        shaderentry("main")
        shadertype("Mesh")
    
    filter "files:**_as.hlsl"
        removeflags("ExcludeFromBuild")
        shaderentry("main")
        shadertype("Amplification")
        
    filter "files:**_cs.hlsl"
        removeflags("ExcludeFromBuild")
        shaderentry("main")
        shadertype("Compute")

    shaderoptions ({"/WX"})
    
