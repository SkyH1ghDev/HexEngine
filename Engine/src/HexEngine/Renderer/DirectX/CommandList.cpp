#include "CommandList.hpp"

CommandList::CommandList(const winrt::com_ptr<ID3D12GraphicsCommandList6>& commandList)
{
    m_commandList = commandList;
}
