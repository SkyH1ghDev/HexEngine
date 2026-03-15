local gcc = premake.tools.gcc

gcc.tools = {
    cc = "/bin/winegcc",
    cxx = "/bin/wineg++",
    ar = "/bin/ar"
    }

function gcc.gettoolname(cfg, tool)
    return gcc.tools[tool]
end