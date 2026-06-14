project "GoogleTest"
    kind "StaticLib"
    location(projectsPath)

    moduleDirectory = AddQuotation(path.getdirectory(_SCRIPT) .. "/%{prj.name}")

    targetdir(targetBuildPath .. "/External")
    objdir(objBuildPath .. "/%{prj.name}")
    
    filter "system:windows"
        kind "Utility"

        filter "configurations:release"
            prebuildcommands{
                "{MKDIR} %{prj.objdir}",
                "cmake -S " .. moduleDirectory .. " -B %{prj.objdir} -DCMAKE_INSTALL_PREFIX=%{prj.targetdir} -DCMAKE_MSVC_RUNTIME_LIBRARY='MultiThreaded'",
                "cmake --build %{prj.objdir} --config %{cfg.buildcfg} --target install",
            }
        filter "configurations:debug"
            prebuildcommands{
                "{MKDIR} %{prj.objdir}",
                "cmake -S " .. moduleDirectory .. " -B %{prj.objdir} -DCMAKE_INSTALL_PREFIX=%{prj.targetdir} -DCMAKE_MSVC_RUNTIME_LIBRARY='MultiThreadedDebug'",
                "cmake --build %{prj.objdir} --config %{cfg.buildcfg} --target install",
            }

    filter "system:linux"
        kind "Makefile"
        buildcommands{
            "{MKDIR} %{prj.objdir}",
            "cmake -S " .. moduleDirectory .. " -B %{prj.objdir} -DCMAKE_INSTALL_PREFIX=%{prj.targetdir} ",
            "cmake --build %{prj.objdir} --config %{cfg.buildcfg} --target install",
        }

project "SDL3"
    kind "StaticLib"
    location(projectsPath)

    moduleDirectory = AddQuotation(path.getdirectory(_SCRIPT) .. "/%{prj.name}")

    targetdir(targetBuildPath .. "/External")
    objdir(objBuildPath .. "/%{prj.name}")

    filter "system:windows"
        kind "Utility"

        filter "configurations:release"
            prebuildcommands{
                "{MKDIR} %{prj.objdir}",
                "cmake -S " .. moduleDirectory .. " -B %{prj.objdir} -DCMAKE_INSTALL_PREFIX=%{prj.targetdir} -DSDL_STATIC=ON -DSDL_SHARED=OFF -DSDL_LIBC=ON -DCMAKE_MSVC_RUNTIME_LIBRARY='MultiThreaded'",
                "cmake --build %{prj.objdir} --config %{cfg.buildcfg} --target install",
            }

        filter "configurations:debug"
            prebuildcommands{
                "{MKDIR} %{prj.objdir}",
                "cmake -S " .. moduleDirectory .. " -B %{prj.objdir} -DCMAKE_INSTALL_PREFIX=%{prj.targetdir} -DSDL_STATIC=ON -DSDL_SHARED=OFF -DSDL_LIBC=ON -DCMAKE_MSVC_RUNTIME_LIBRARY='MultiThreadedDebug'",
                "cmake --build %{prj.objdir} --config %{cfg.buildcfg} --target install",
            }

    filter "system:linux"
        kind "Makefile"
        buildcommands{
            "{MKDIR} %{prj.objdir}",
            "cmake -S " .. moduleDirectory .. " -B %{prj.objdir} -DCMAKE_INSTALL_PREFIX=%{prj.targetdir} -DSDL_STATIC=ON -DSDL_SHARED=OFF -DSDL_LIBC=ON",
            "cmake --build %{prj.objdir} --config %{cfg.buildcfg} --target install",
        }


project "VulkanHeaders"
    kind "StaticLib"
    location(projectsPath)

    moduleDirectory = AddQuotation(path.getdirectory(_SCRIPT) .. "/%{prj.name}")

    targetdir(targetBuildPath .. "/External")
    objdir(objBuildPath .. "/%{prj.name}")

    filter "system:windows"
        kind "Utility"

        filter "configurations:release"
            prebuildcommands{
                "{MKDIR} %{prj.objdir}",
                "cmake -S " .. moduleDirectory .. " -B %{prj.objdir} -DCMAKE_INSTALL_PREFIX=%{prj.targetdir} -DCMAKE_MSVC_RUNTIME_LIBRARY='MultiThreaded'",
                "cmake --build %{prj.objdir} --config %{cfg.buildcfg} --target install",
            }

        filter "configurations:debug"
            prebuildcommands{
                "{MKDIR} %{prj.objdir}",
                "cmake -S " .. moduleDirectory .. " -B %{prj.objdir} -DCMAKE_INSTALL_PREFIX=%{prj.targetdir} -DCMAKE_MSVC_RUNTIME_LIBRARY='MultiThreadedDebug'",
                "cmake --build %{prj.objdir} --config %{cfg.buildcfg} --target install",
            }

    filter "system:linux"
        kind "Makefile"
        buildcommands{
            "{MKDIR} %{prj.objdir}",
            "cmake -S " .. moduleDirectory .. " -B %{prj.objdir} -DCMAKE_INSTALL_PREFIX=%{prj.targetdir}",
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
        "ImGui/backends/imgui_impl_sdl3.cpp"
    }

    includedirs{
        "ImGui/",
        "ImGui/backends/",
        targetBuildPath .. "/External/include/"
    }

    imGuiPath = targetBuildPath .. "/External/include/%{prj.name}"

    filter "system:windows"
        prebuildcommands{
            "{MKDIR} " .. AddQuotation(imGuiPath),
            "{COPY} " .. AddQuotation(rootPath).. "/External/ImGui/*.h " .. AddQuotation(imGuiPath),
            "{COPY} " .. AddQuotation(rootPath) .. "/External/ImGui/backends/imgui_impl_dx12.h " .. AddQuotation(imGuiPath),
            "{COPY} " .. AddQuotation(rootPath) .. "/External/ImGui/backends/imgui_impl_SDL3.h " .. AddQuotation(imGuiPath)
        }

    filter "system:linux"

        includedirs{
            "ImGui/",
            "ImGui/backends/",
            targetBuildPath .. "/External/include/",
        }

        buildcommands{
            "{MKDIR} " .. AddQuotation(imGuiPath),
            "{COPY} " .. AddQuotation(rootPath) .. "/External/ImGui/*.h " .. AddQuotation(imGuiPath),
            "{COPY} " .. AddQuotation(rootPath) .. "/External/ImGui/backends/imgui_impl_dx12.h " .. AddQuotation(imGuiPath),
            "{COPY} " .. AddQuotation(rootPath) .. "/External/ImGui/backends/imgui_impl_SDL3.h " .. AddQuotation(imGuiPath)
        }
