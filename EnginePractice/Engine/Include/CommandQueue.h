#pragma once
class CCommandQueue
{
public:

	void Init(ID3D12Device* device, class CSwapChain* swapChain);

	void WaitGpuComplete();

private:
	ID3D12CommandQueue			*m_cmdQueue;
	ID3D12CommandAllocator		*m_cmdAlloc;
	ID3D12GraphicsCommandList	*m_cmdList;

	ID3D12Fence					*m_fence;
	UINT64						m_fenceValue;
	HANDLE						m_fenceEvent;

	CSwapChain					*m_swapChain;
};

