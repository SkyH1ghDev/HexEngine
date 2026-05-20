#include "CommandList.hpp"

using namespace HexEngine::Graphics::DirectX12;

CommandList::CommandList(const winrt::com_ptr<ID3D12GraphicsCommandList6>& commandList)
{
    m_commandList = commandList;
}
