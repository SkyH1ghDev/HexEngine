#pragma once
#define WIN32_LEAN_AND_MEAN

#include <exception>
#include <Windows.h>
#include <string>

namespace DirectXUtils
{
    // Helper class for COM exceptions
    class DirectXException : public std::exception
    {
    public:
        DirectXException(HRESULT hr);
        const char* what() const noexcept override;

    private:
        HRESULT m_result;
        std::string m_message;
    };

    // Utility Functions
    bool CheckTearingSupport();
    
    inline void ThrowIfFailed(HRESULT hr)
    {
        if (FAILED(hr))
        {
            throw DirectXException(hr);
        }
    }
}