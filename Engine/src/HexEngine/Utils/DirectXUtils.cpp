#include "DirectXUtils.hpp"
#include <winrt/base.h>
#include <format>

using namespace winrt;

DirectXUtils::DirectXException::DirectXException(HRESULT hr) :
    mResult(hr),
    mMessage(std::format("Failure with HRESULT of {:08X}", static_cast<std::uint32_t>(mResult)))
{
    
}

const char* DirectXUtils::DirectXException::what() const noexcept
{
    return mMessage.c_str();
}