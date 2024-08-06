#include "EngineInfo.h"
#include "ConstantBuffer.h"

CConstantBuffer::CConstantBuffer()
{
}

CConstantBuffer::~CConstantBuffer()
{
	if (m_cbvBuffer)
	{
		if (m_cbvBuffer != nullptr)
			m_cbvBuffer->Unmap(0, nullptr);

		m_cbvBuffer = nullptr;
	}
}

D3D12_GPU_VIRTUAL_ADDRESS CConstantBuffer::GetGpuVirtualAddress(UINT32 index)
{
	D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = m_cbvBuffer->GetGPUVirtualAddress();
	objCBAddress += index * m_elementSize;
	return objCBAddress;
}

D3D12_CPU_DESCRIPTOR_HANDLE CConstantBuffer::GetCpuHandle(UINT32 index)
{
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(m_cpuHandleBegin, index * m_handleIncrementSize);
}

void CConstantBuffer::Init(ID3D12Device* device, UINT32 size, UINT32 count)
{
	m_elementSize = (size + 255) & ~255;
	m_elementCount = count;

	CreateBuffer(device);
	CreateView(device);
}

void CConstantBuffer::Clear()
{
	m_currentIndex = 0;
}

void CConstantBuffer::PushData(void* buffer, UINT32 size)
{
	assert(m_currentIndex < m_elementCount);
	assert(m_elementSize == ((size + 255) & ~255));

	::memcpy(&m_mappedBuffer[m_currentIndex * m_elementSize], buffer, size);

	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = GetCpuHandle(m_currentIndex);
	//GEngine->GetTableDescHeap()->SetCBV(cpuHandle, _reg);

	m_currentIndex++;
}

void CConstantBuffer::PushConstantBufferViewData(ID3D12GraphicsCommandList* cmdList, void* buffer, UINT32 size)
{
	assert(m_elementSize == ((size + 255) & ~255));
	::memcpy(&m_mappedBuffer[0], buffer, size);
	cmdList->SetGraphicsRootConstantBufferView(0, GetGpuVirtualAddress(0));
}

void CConstantBuffer::CreateBuffer(ID3D12Device* device)
{
	UINT32 bufferSize = m_elementSize * m_elementCount;
	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);
	D3D12_CLEAR_VALUE clearValue = CD3DX12_CLEAR_VALUE();

	HRESULT hr = device->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_cbvBuffer));

	m_cbvBuffer->Map(0, nullptr, reinterpret_cast<void**>(&m_mappedBuffer));
}

void CConstantBuffer::CreateView(ID3D12Device* device)
{	
	D3D12_DESCRIPTOR_HEAP_DESC cbvDesc = {};
	cbvDesc.NumDescriptors = m_elementCount;
	cbvDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	cbvDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	device->CreateDescriptorHeap(&cbvDesc, IID_PPV_ARGS(&m_cbvHeap));

	m_cpuHandleBegin = m_cbvHeap->GetCPUDescriptorHandleForHeapStart();
	m_handleIncrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	for (UINT32 i = 0; i < m_elementCount; ++i)
	{
		D3D12_CPU_DESCRIPTOR_HANDLE cbvHandle = GetCpuHandle(i);

		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = m_cbvBuffer->GetGPUVirtualAddress() + static_cast<UINT64>(m_elementSize) * i;
		cbvDesc.SizeInBytes = m_elementSize;

		device->CreateConstantBufferView(&cbvDesc, cbvHandle);
	}
}

