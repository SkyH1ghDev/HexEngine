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
        "src/**.hlsl",
        "assets/**.hlsl"
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

    filter "files:**/vs_**.hlsl"
        removeflags("ExcludeFromBuild")
        shaderentry("main")
        shadertype("Vertex")
    
    filter "files:**/hs_**.hlsl"
        removeflags("ExcludeFromBuild")
        shaderentry("main")
        shadertype("Hull")
    
    filter "files:**/ds_**.hlsl"
        removeflags("ExcludeFromBuild")
        shaderentry("main")
        shadertype("Domain")
    
    filter "files:**/gs_**.hlsl"
        removeflags("ExcludeFromBuild")
        shaderentry("main")
        shadertype("Geometry")
    
    filter "files:**/ps_**.hlsl"
        removeflags("ExcludeFromBuild")
        shaderentry("main")
        shadertype("Pixel")
    
    filter "files:**/ms_**.hlsl"
        removeflags("ExcludeFromBuild")
        shaderentry("main")
        shadertype("Mesh")
    
    filter "files:**/as_**.hlsl"
        removeflags("ExcludeFromBuild")
        shaderentry("main")
        shadertype("Amplification")
        
    filter "files:**/cs_**.hlsl"
        removeflags("ExcludeFromBuild")
        shaderentry("main")
        shadertype("Compute")

    shaderoptions ({"/WX"})
    
