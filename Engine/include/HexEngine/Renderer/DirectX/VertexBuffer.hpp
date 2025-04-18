#pragma once
#include <HexEngine/Renderer/DirectX/Resource.hpp>

class VertexBuffer {
public:
    VertexBuffer() = default;
    ~VertexBuffer() = default;
    VertexBuffer(const VertexBuffer& other) = default;
    VertexBuffer& operator=(const VertexBuffer& other) = default;
    VertexBuffer(VertexBuffer&& other) noexcept = default;
    VertexBuffer& operator=(VertexBuffer&& other) noexcept = default;

private:
    Resource m_vertexBuffer;
    D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
};
