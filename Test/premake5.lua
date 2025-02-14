project "Test"
    kind "ConsoleApp"
    targetdir(targetBuildPath .. "/%{prj.name}")
    objdir(objBuildPath .. "/%{prj.name}")
    files {"src/**.hpp", "src/**.cpp"}
    includedirs{"../Engine/include", targetBuildPath .. "/External/include"}

    libdirs{targetBuildPath .. "/External/lib"}

    -- Although this consoleApp does not link to the "googletest" project per say, it is still dependant on its build to finish
    dependson {"googletest"}

    links{"Engine", "gtest"}