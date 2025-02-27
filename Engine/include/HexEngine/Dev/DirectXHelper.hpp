#pragma once
#define WIN32_LEAN_AND_MEAN

#include <exception>
#include <cstdio>
#include <Windows.h>

namespace DirectX
{
    // Helper class for COM exceptions
    class ComException : public std::exception
    {
    public:
        ComException(HRESULT hr) : m_result(hr) {}

        const char* what() const noexcept override
        {
            static char sStr[64] = {};
            static_cast<void>
            (
                sprintf_s
                (
                    sStr,
                    "Failure with HRESULT of %08X",
                    static_cast<unsigned int>(m_result)
                )
            );
            return sStr;
        }

    private:
        HRESULT m_result;
    };

    // Helper utility converts D3D API failures into exceptions.
    inline void ThrowIfFailed(HRESULT hr)
    {
        if (FAILED(hr))
        {
            throw ComException(hr);
        }
    }
}