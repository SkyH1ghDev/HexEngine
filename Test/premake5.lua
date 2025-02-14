project "Test"

    location(projectsPath)

    kind "ConsoleApp"
    targetdir(targetBuildPath .. "/%{prj.name}")
    objdir(objBuildPath .. "/%{prj.name}")
    files {"src/**.hpp", "src/**.cpp"}
    includedirs{"../Engine/include", targetBuildPath .. "/External/include"}

    libdirs{targetBuildPath .. "/External/lib"}

    -- Although this consoleApp does not link to the "GoogleTest" project per say, it is still dependant on its build to finish
    dependson {"GoogleTest"}

    links{"Engine", "gtest"}