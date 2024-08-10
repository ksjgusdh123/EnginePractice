#include "EngineInfo.h"
#include "Mesh.h"

CMesh::CMesh()
{
}

CMesh::~CMesh()
{
	if (m_vertexBuffer) m_vertexBuffer->Release();
	if (m_indexBuffer) m_indexBuffer->Release();
}

void CMesh::Init(ID3D12Device* device, const std::vector<Vertex>& vertexBuffer)
{
	CreateVertexBuffer(device, vertexBuffer);
}

void CMesh::Init(ID3D12Device* device, const std::vector<Vertex>& vertexBuffer, const std::vector<UINT32>& indexBuffer)
{
	CreateVertexBuffer(device, vertexBuffer);
	CreateIndexBuffer(device, indexBuffer);
}

void CMesh::Render(ID3D12GraphicsCommandList* cmdList)
{
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	cmdList->IASetVertexBuffers(0, 1, &m_vertexBufferView);

	if (m_indexBuffer)
	{
		cmdList->IASetIndexBuffer(&m_indexBufferView);
		cmdList->DrawIndexedInstanced(m_indexCount, 1, 0, 0, 0);
	}
	else
	{
		cmdList->DrawInstanced(m_vertexCount, 1, 0, 0);
	}
}

void CMesh::CreateVertexBuffer(ID3D12Device* device, const std::vector<Vertex>& buffer)
{
	m_vertexCount = static_cast<UINT32>(buffer.size());
	UINT32 bufferSize = m_vertexCount * sizeof(Vertex);

	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	device->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_vertexBuffer));

	// Copy the triangle data to the vertex buffer.
	void* vertexDataBuffer = nullptr;
	CD3DX12_RANGE readRange(0, 0); // We do not intend to read from this resource on the CPU.
	m_vertexBuffer->Map(0, &readRange, &vertexDataBuffer);
	::memcpy(vertexDataBuffer, &buffer[0], bufferSize);
	m_vertexBuffer->Unmap(0, nullptr);

	// Initialize the vertex buffer view.
	m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
	m_vertexBufferView.StrideInBytes = sizeof(Vertex); // 정점 1개 크기
	m_vertexBufferView.SizeInBytes = bufferSize; // 버퍼의 크기
}

void CMesh::CreateIndexBuffer(ID3D12Device* device, const std::vector<UINT32>& buffer)
{
	m_indexCount = static_cast<UINT32>(buffer.size());
	UINT32 bufferSize = m_indexCount * sizeof(UINT32);

	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	device->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_indexBuffer));

	void* indexDataBuffer = nullptr;
	CD3DX12_RANGE readRange(0, 0); // We do not intend to read from this resource on the CPU.
	m_indexBuffer->Map(0, &readRange, &indexDataBuffer);
	::memcpy(indexDataBuffer, &buffer[0], bufferSize);
	m_indexBuffer->Unmap(0, nullptr);

	m_indexBufferView.BufferLocation = m_indexBuffer->GetGPUVirtualAddress();
	m_indexBufferView.Format = DXGI_FORMAT_R32_UINT;
	m_indexBufferView.SizeInBytes = bufferSize;
}
