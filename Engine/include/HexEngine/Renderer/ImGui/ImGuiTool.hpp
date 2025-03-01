#pragma once

class ImGuiTool {
public:
    ImGuiTool() = default;
    ~ImGuiTool() = default;
    ImGuiTool(const ImGuiTool& other) = default;
    ImGuiTool& operator=(const ImGuiTool& other) = default;
    ImGuiTool(ImGuiTool&& other) noexcept = default;
    ImGuiTool& operator=(ImGuiTool&& other) noexcept = default;

};