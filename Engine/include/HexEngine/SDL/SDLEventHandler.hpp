#pragma once

class SDLEventHandler {
public:
    SDLEventHandler() = default;
    ~SDLEventHandler() = default;
    SDLEventHandler(const SDLEventHandler& other) = default;
    SDLEventHandler& operator=(const SDLEventHandler& other) = default;
    SDLEventHandler(SDLEventHandler&& other) noexcept = default;
    SDLEventHandler& operator=(SDLEventHandler&& other) noexcept = default;

    static void HandleEvents();
};