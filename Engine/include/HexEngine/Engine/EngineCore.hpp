#pragma once

//| Base Class
/*|
 *  Base Class v2
 */
class EngineCore
{
public:
    // Base Constructor
    EngineCore() = default;
    // Base Destructor
    ~EngineCore() = default;
    // Deleted Copy-Constructor
    EngineCore(const EngineCore& other) = delete;
    // Deleted Copy-Assignment
    EngineCore& operator=(const EngineCore& other) = delete;
    // Deleted Move-Constructor
    EngineCore(EngineCore&& other) noexcept = delete;
    // Deleted Move-Assignment
    EngineCore& operator=(EngineCore&& other) noexcept = delete;

    //| Run func
    /*|
     *  Thingy-mabob
     */
    static void Run();
    static void Quit();
    
    static void PostQuitEvent_Callback(bool keyState);

private:
    inline static bool m_shouldRun {true};
};
