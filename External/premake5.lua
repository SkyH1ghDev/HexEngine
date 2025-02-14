moduleDirectory = "\"" .. path.getdirectory(_SCRIPT) .. "\"" .. "/%{prj.name}"

project "GoogleTest"

    configurations {}
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

    configurations{}
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
