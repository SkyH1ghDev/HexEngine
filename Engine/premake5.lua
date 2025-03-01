project "Engine"
    kind "StaticLib"

    location(projectsPath)

    targetdir(targetBuildPath .. "/%{prj.name}")
    objdir(objBuildPath .. "/%{prj.name}")
    files {"include/**.hpp", "src/**.cpp"}
    includedirs {"include/", "include/**",  targetBuildPath .. "/External/include/"}
    libdirs {targetBuildPath .. "/External/lib/"}

    dependson{"SDL3", "ImGui"}