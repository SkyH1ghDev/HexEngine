#pragma once

class IndexBuffer {
public:
    IndexBuffer() = default;
    ~IndexBuffer() = default;
    IndexBuffer(const IndexBuffer& other) = default;
    IndexBuffer& operator=(const IndexBuffer& other) = default;
    IndexBuffer(IndexBuffer&& other) noexcept = default;
    IndexBuffer& operator=(IndexBuffer&& other) noexcept = default;

};