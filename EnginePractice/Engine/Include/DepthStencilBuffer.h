#pragma once
class CDepthStencilBuffer
{
public:
	D3D12_CPU_DESCRIPTOR_HANDLE GetDsvCpuHandle() { return m_dsvHandle; }
	DXGI_FORMAT GetFormat() { return m_dsvFormat; }

public:
	void Init(ID3D12Device* device, ScreenInfo& info, DXGI_FORMAT format = DXGI_FORMAT_D24_UNORM_S8_UINT);
	
private:
	ID3D12Resource				*m_dsvBuffer;
	ID3D12DescriptorHeap		*m_dsvHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE  m_dsvHandle;
	DXGI_FORMAT					 m_dsvFormat;
};

