local dirs = {"./Build", "./.vscode", "./.vs", "./.idea", "./Generated"}

newaction {
    trigger = "clean",
    description = "clean files generated by premake5",
    
    --[[os.execute(
        "{RMDIR} ./Build",
        "{RMDIR} ./.vscode",
        "{RMDIR} ./.vs",
        "{RMDIR} ./.idea",
        "{RMDIR} ./Generated"
    )]]
    
    execute = function()
        for _, dir in ipairs(dirs) do
            if os.isdir(dir) then
                os.execute(
                    "{RMDIR} " .. dir
                )
                
                --[[local t, err = os.rmdir(dir)
                print(err)]]
            end
        end
        
        os.remove("*.sln")
    end
    
    --[[execute = function()
        local t, error = os.rmdir("./Build")
        print(error)
        os.rmdir("./.vscode")
        os.rmdir("./.vs")
        os.rmdir("./Generated")
    end]]
}