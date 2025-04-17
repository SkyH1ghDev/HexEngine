local msvcRTLibFlag = nil
filter "configurations:debug"
    msvcRTLibFlag = "-DCMAKE_MSVC_RUNTIME_LIBRARY='MultiThreadedDebug'"

filter "configurations:release"
    msvcRTLibFlag = "-DCMAKE_MSVC_RUNTIME_LIBRARY='MultiThreaded'"

project "GoogleTest"
    kind "StaticLib"

    moduleDirectory = "\"" .. path.getdirectory(_SCRIPT) .. "\"" .. "/%{prj.name}"
    location(projectsPath)

    targetdir(targetBuildPath .. "/External")
    objdir(objBuildPath .. "/%{prj.name}")
    
    filter "system:windows"
        kind "Utility"
        prebuildcommands{
            "{MKDIR} %{prj.objdir}",
            "cmake -S " .. moduleDirectory .. " -B %{prj.objdir} -DCMAKE_INSTALL_PREFIX=%{prj.targetdir} " .. msvcRTLibFlag,
            "cmake --build %{prj.objdir} --config %{cfg.buildcfg} --target install",
        }

    filter "system:linux"
        kind "Makefile"
        buildcommands{
            "{MKDIR} %{prj.objdir}",
            "cmake -S " .. moduleDirectory .. " -B %{prj.objdir} -DCMAKE_INSTALL_PREFIX=%{prj.targetdir}",
            "cmake --build %{prj.objdir} --config %{cfg.buildcfg} --target install",
        }

project "SDL3"
    kind "StaticLib"

    moduleDirectory = "\"" .. path.getdirectory(_SCRIPT) .. "\"" .. "/%{prj.name}"
    location(projectsPath)

    targetdir(targetBuildPath .. "/External")
    objdir(objBuildPath .. "/%{prj.name}")

    filter "system:windows"
        kind "Utility"
        prebuildcommands{
            "{MKDIR} %{prj.objdir}",
            "cmake -S " .. moduleDirectory .. " -B %{prj.objdir} -DCMAKE_INSTALL_PREFIX=%{prj.targetdir} -DSDL_STATIC=ON -DSDL_SHARED=OFF -DSDL_LIBC=ON " .. msvcRTLibFlag,
            "cmake --build %{prj.objdir} --config %{cfg.buildcfg} --target install",
        }

    filter "system:linux"
        kind "Makefile"
        buildcommands{
            "{MKDIR} %{prj.objdir}",
            "cmake -S " .. moduleDirectory .. " -B %{prj.objdir} -DCMAKE_INSTALL_PREFIX=%{prj.targetdir} -DSDL_STATIC=ON -DSDL_SHARED=OFF -DSDL_LIBC=ON",
            "cmake --build %{prj.objdir} --config %{cfg.buildcfg} --target install",
        }

project "DirectXToolKit"
    kind "StaticLib"

    moduleDir = "\"" .. path.getdirectory(_SCRIPT) .. "\"" .. "/%{prj.name}"
    location(projectsPath)

    targetdir(targetBuildPath .. "/External")
    objdir(objBuildPath .. "/%{prj.name}")


    filter "system:windows"
        kind "Utility"
        prebuildcommands
        {
            "{MKDIR} %{prj.objdir}",
            "cmake -S " .. moduleDir .. " -B %{prj.objdir} -DCMAKE_INSTALL_PREFIX=%{prj.targetdir} " .. msvcRTLibFlag,
            "cmake --build %{prj.objdir} --config %{cfg.buildcfg} --target install",
        }

    filter "system:linux"
        kind "Makefile"
        buildcommands
        {
            "{MKDIR} %{prj.objdir}",
            "cmake -S " .. moduleDir .. " -B %{prj.objdir} -DCMAKE_INSTALL_PREFIX=%{prj.targetdir}",
            "cmake --build %{prj.objdir} --config %{cfg.buildcfg} --target install",
        }

project "DirectXHeaders"
    kind "StaticLib"

    moduleDir = "\"" .. path.getdirectory(_SCRIPT) .. "\"" .. "/%{prj.name}"
    location(projectsPath)

    targetdir(targetBuildPath .. "/External")
    objdir(objBuildPath .. "/%{prj.name}")

    filter "system:windows"
    kind "Utility"
        prebuildcommands
        {
            "{MKDIR} %{prj.objdir}",
            "cmake -S " .. moduleDir .. " -B %{prj.objdir} -DCMAKE_INSTALL_PREFIX=%{prj.targetdir} -DDXHEADERS_BUILD_TEST=FALSE -DDXHEADERS_BUILD_GOOGLE_TEST=FALSE " .. msvcRTLibFlag,
            "cmake --build %{prj.objdir} --config %{cfg.buildcfg} --target install",
        }

    filter "system:linux"
    kind "Makefile"
        buildcommands
        {
            "{MKDIR} %{prj.objdir}",
            "cmake -S " .. moduleDir .. " -B %{prj.objdir} -DCMAKE_INSTALL_PREFIX=%{prj.targetdir}",
            "cmake --build %{prj.objdir} --config %{cfg.buildcfg} --target install",
        }

project "ImGui"
    kind "StaticLib"
    location(projectsPath)

    warnings "Off"

    targetdir(targetBuildPath .. "/External/lib/")
    objdir(objBuildPath .. "/%{prj.name}")

    dependson("SDL3")

    files {
        "ImGui/imgui*.cpp",
        "ImGui/backends/imgui_impl_dx12.cpp",
        "ImGui/backends/imgui_impl_SDL3.cpp"
    }

    includedirs{
        "ImGui/",
        "ImGui/backends/",
        targetBuildPath .. "/External/include/"
    }

    mkdirPath = "\"" .. targetBuildPath .. "/External/include/%{prj.name}\""
    copyPath = "\"" .. targetBuildPath .. "/External/include/%{prj.name}\""

    prebuildcommands{
        "{MKDIR} " .. mkdirPath,
        "{COPY} " .. rootPath .. "/External/ImGui/*.h " .. copyPath,
        "{COPY} " .. rootPath .. "/External/ImGui/backends/imgui_impl_dx12.h " .. copyPath,
        "{COPY} " .. rootPath .. "/External/ImGui/backends/imgui_impl_SDL3.h " .. copyPath
    }
