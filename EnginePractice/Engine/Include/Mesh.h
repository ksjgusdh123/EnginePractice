#pragma once

// temp
struct Vertex
{
	XMFLOAT3 position;
	XMFLOAT4 color;
};

class CMesh
{
public:
	CMesh();
	virtual ~CMesh();

public:
	void Init(ID3D12Device* device, const std::vector<Vertex>& vertexBuffer);
	void Init(ID3D12Device* device, const std::vector<Vertex>& vertexBuffer, const std::vector<UINT32>& indexBuffer);
	void Render(ID3D12GraphicsCommandList* cmdList);

private:
	void CreateVertexBuffer(ID3D12Device* device, const std::vector<Vertex>& buffer);
	void CreateIndexBuffer(ID3D12Device* device, const std::vector<UINT32>& buffer);

private:
	ID3D12Resource				*m_vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW	m_vertexBufferView = {};
	UINT32						m_vertexCount = 0;

	ID3D12Resource				*m_indexBuffer;
	D3D12_INDEX_BUFFER_VIEW		m_indexBufferView;
	UINT32						m_indexCount = 0;
};

