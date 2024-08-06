#pragma once
class CConstantBuffer
{
public:
	CConstantBuffer();
	~CConstantBuffer();

public:
	D3D12_GPU_VIRTUAL_ADDRESS GetGpuVirtualAddress(UINT32 index);
	D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle(UINT32 index);

public:
	void Init(ID3D12Device* device, UINT32 size, UINT32 count);
	void Clear();

	void PushData(void* buffer, UINT32 size);
	void PushConstantBufferViewData(ID3D12GraphicsCommandList* cmdList, void* buffer, UINT32 size);

private:
	void CreateBuffer(ID3D12Device* device);
	void CreateView(ID3D12Device* device);

private:
	ID3D12Resource				*m_cbvBuffer;
	BYTE						*m_mappedBuffer = nullptr;
	UINT32						m_elementSize = 0;
	UINT32						m_elementCount = 0;

	ID3D12DescriptorHeap		*m_cbvHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE m_cpuHandleBegin;
	UINT32						m_handleIncrementSize;

	UINT32						m_currentIndex;
};

