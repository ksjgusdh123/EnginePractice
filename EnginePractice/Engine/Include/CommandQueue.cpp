#include "EngineInfo.h"
#include "CommandQueue.h"
#include "SwapChain.h"

void CCommandQueue::Init(ID3D12Device* device, CSwapChain* swapChain)
{
	m_swapChain = swapChain;

	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc;
	::ZeroMemory(&cmdQueueDesc, sizeof(D3D12_COMMAND_QUEUE_DESC));
	cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	HRESULT hResult = device->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&m_cmdQueue));

	hResult = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_cmdAlloc));

	hResult = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_cmdAlloc, NULL, IID_PPV_ARGS(&m_cmdList));
	hResult = m_cmdList->Close();

	device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence));
	m_fenceEvent = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);
}

void CCommandQueue::WaitGpuComplete()
{
	UINT64 nFenceValue = ++m_fenceValue;
	HRESULT hResult = m_cmdQueue->Signal(m_fence, nFenceValue);
	if (m_fence->GetCompletedValue() < nFenceValue)
	{
		hResult = m_fence->SetEventOnCompletion(nFenceValue, m_fenceEvent);
		::WaitForSingleObject(m_fenceEvent, INFINITE);
	}
}
