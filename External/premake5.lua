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
            "cmake -S " .. moduleDirectory .. " -B %{prj.objdir} -DCMAKE_INSTALL_PREFIX=%{prj.targetdir}",
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
            "cmake -S " .. moduleDirectory .. " -B %{prj.objdir} -DCMAKE_INSTALL_PREFIX=%{prj.targetdir} -DCMAKE_MSVC_RUNTIME_LIBRARY='MultiThreadedDebug' -DSDL_STATIC=ON -DSDL_SHARED=OFF -DSDL_LIBC=ON",
            "cmake --build %{prj.objdir} --config %{cfg.buildcfg} --target install",
        }

    filter "system:linux"
        kind "Makefile"
        buildcommands{
            "{MKDIR} %{prj.objdir}",
            "cmake -S " .. moduleDirectory .. " -B %{prj.objdir} -DCMAKE_INSTALL_PREFIX=%{prj.targetdir} -DCMAKE_MSVC_RUNTIME_LIBRARY='MultiThreadedDebug' -DSDL_STATIC=ON -DSDL_SHARED=OFF -DSDL_LIBC=ON",
            "cmake --build %{prj.objdir} --config %{cfg.buildcfg} --target install",
        }

project "DirectXToolKit"
    targetdir(targetBuildPath .. "/External")
    objdir(objBuildPath .. "/%{prj.name}")

    libraryDir = "\"" .. path.getdirectory(_SCRIPT) .. "\"" .. "/%{prj.name}"

    filter "system:windows"
        kind "Utility"
        prebuildcommands
        {
            "{MKDIR} %{prj.objdir}",
            "cmake -S " .. libraryDir .. " -B %{prj.objdir} -DCMAKE_INSTALL_PREFIX=%{prj.targetdir} -DCMAKE_MSVC_RUNTIME_LIBRARY='MultiThreadedDebug'",
            "cmake --build %{prj.objdir} --config %{cfg.buildcfg} --target install",
        }

    filter "system:linux"
        kind "Makefile"
        buildcommands
        {
            "{MKDIR} %{prj.objdir}",
            "cmake -S " .. libraryDir .. " -B %{prj.objdir} -DCMAKE_INSTALL_PREFIX=%{prj.targetdir}",
            "cmake --build %{prj.objdir} --config %{cfg.buildcfg} --target install",
        }

project "DirectXHeaders"
    targetdir(targetBuildPath .. "/External")
    objdir(objBuildPath .. "/%{prj.name}")

    libraryDir = "\"" .. path.getdirectory(_SCRIPT) .. "\"" .. "/%{prj.name}"

    filter "system:windows"
    kind "Utility"
        prebuildcommands
        {
            "{MKDIR} %{prj.objdir}",
            "cmake -S " .. libraryDir .. " -B %{prj.objdir} -DCMAKE_INSTALL_PREFIX=%{prj.targetdir} -DCMAKE_MSVC_RUNTIME_LIBRARY='MultiThreadedDebug'",
            "cmake --build %{prj.objdir} --config %{cfg.buildcfg} --target install",
        }

    filter "system:linux"
    kind "Makefile"
        buildcommands
        {
            "{MKDIR} %{prj.objdir}",
            "cmake -S " .. libraryDir .. " -B %{prj.objdir} -DCMAKE_INSTALL_PREFIX=%{prj.targetdir}",
            "cmake --build %{prj.objdir} --config %{cfg.buildcfg} --target install",
        }

project "ImGui"
    kind "StaticLib"
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
        "{COPY} ../External/ImGui/*.h " .. copyPath,
        "{COPY} ../External/ImGui/backends/imgui_impl_dx12.h " .. copyPath,
        "{COPY} ../External/ImGui/backends/imgui_impl_SDL3.h " .. copyPath
    }
