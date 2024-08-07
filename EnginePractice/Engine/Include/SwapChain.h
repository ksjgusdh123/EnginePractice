#pragma once
class CSwapChain
{
public:
	ID3D12Resource* GetBackRTVBuffer() { return m_rtvBuffer[m_backBufferIndex]; }
	D3D12_CPU_DESCRIPTOR_HANDLE GetBackRTV() { return m_rtvHandle[m_backBufferIndex]; }

public:
	void Init(ID3D12Device* device, IDXGIFactory* factory, ID3D12CommandQueue* cmdQueue, ScreenInfo& info, bool windowed = true);
	void OnDestroy();

	void Present();
	void SwapIndex();
private:
	void CreateSwapChain(IDXGIFactory* factory, ID3D12CommandQueue* cmdQueue, ScreenInfo& info, bool windowed);
	void CreateRTV(ID3D12Device* device);


private:
	IDXGISwapChain3				*m_swapChain;
	ID3D12Resource				*m_rtvBuffer[SWAP_CHAIN_BUFFER_COUNT];
	ID3D12DescriptorHeap		*m_rtvHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE m_rtvHandle[SWAP_CHAIN_BUFFER_COUNT];

	UINT32						m_backBufferIndex = 0;
};

