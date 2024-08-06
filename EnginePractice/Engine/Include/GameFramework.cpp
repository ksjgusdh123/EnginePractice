#include "EngineInfo.h"
#include "GameFramework.h"
#include "Device.h"
#include "SwapChain.h"
#include "CommandQueue.h"
#include "DepthStencilBuffer.h"
#include "RootSignature.h"
#include "Shader.h"

CGameFramework::CGameFramework(HINSTANCE hInst, HWND hWnd, int windowWidth, int windowHeight)
	: m_hInst(hInst), m_hWnd(hWnd)
{
	m_screenInfo.width = windowWidth;
	m_screenInfo.height = windowHeight;
	m_screenInfo.hWnd = hWnd;

	m_device = new CDevice();
	m_swapChain = new CSwapChain();
	m_cmdQueue = new CCommandQueue();
	m_depthStencilBuffer = new CDepthStencilBuffer();
	m_rootSignature = new CRootSignature();
	m_shader = new CShader();
}

bool CGameFramework::Init()
{
	m_viewport = { 0, 0, static_cast<FLOAT>(m_screenInfo.width), static_cast<FLOAT>(m_screenInfo.height), 0.0f, 1.0f };
	m_scissorRect = CD3DX12_RECT(0, 0, m_screenInfo.width, m_screenInfo.height);

	m_device->Init();
	m_cmdQueue->Init(m_device->GetDevice(), m_swapChain);
	CheckMsaa();
	m_swapChain->Init(m_device->GetDevice(), m_device->GetDXGIFactory(), m_cmdQueue->GetCmdQueue(), m_screenInfo);
	m_depthStencilBuffer->Init(m_device->GetDevice(), m_screenInfo);
	m_rootSignature->Init();

	ShaderInfo info =
	{
		RASTERIZER_TYPE::CULL_NONE,
		DEPTH_STENCIL_TYPE::LESS_EQUAL
	};
	m_shader->Init(L"..\\..\\Engine\\Include\\shader.hlsl", info);


	// Create the vertex buffer.
	{
		// Define the geometry for a triangle.
		Vertex triangleVertices[] =
		{
			{ { 0.0f, 0.25f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
			{ { 0.25f, -0.25f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
			{ { -0.25f, -0.25f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }
		};

		const UINT vertexBufferSize = sizeof(triangleVertices);

		// Note: using upload heaps to transfer static data like vert buffers is not 
		// recommended. Every time the GPU needs it, the upload heap will be marshalled 
		// over. Please read up on Default Heap usage. An upload heap is used here for 
		// code simplicity and because there are very few verts to actually transfer.
		D3D12_HEAP_PROPERTIES prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		D3D12_RESOURCE_DESC re = CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize);
		(m_device->GetDevice()->CreateCommittedResource(
			&prop,
			D3D12_HEAP_FLAG_NONE,
			&re,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_vertexBuffer)));

		// Copy the triangle data to the vertex buffer.
		UINT8* pVertexDataBegin;
		CD3DX12_RANGE readRange(0, 0);        // We do not intend to read from this resource on the CPU.
		(m_vertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin)));
		memcpy(pVertexDataBegin, triangleVertices, sizeof(triangleVertices));
		m_vertexBuffer->Unmap(0, nullptr);

		// Initialize the vertex buffer view.
		m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
		m_vertexBufferView.StrideInBytes = sizeof(Vertex);
		m_vertexBufferView.SizeInBytes = vertexBufferSize;
	}

	return true;
}

void CGameFramework::Update()
{
	Render();
}

void CGameFramework::Render()
{
	RenderBegin();
	m_shader->Update();
	RenderEnd();
}

void CGameFramework::RenderBegin()
{
	m_cmdQueue->RenderBegin(&m_viewport, &m_scissorRect);
	m_cmdQueue->GetCmdList()->IASetVertexBuffers(0, 1, &m_vertexBufferView);

}

void CGameFramework::RenderEnd()
{
	m_cmdQueue->RenderEnd();
}

void CGameFramework::CheckMsaa()
{
	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS d3dMsaaQualityLevels;
	d3dMsaaQualityLevels.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dMsaaQualityLevels.SampleCount = 4;
	d3dMsaaQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	d3dMsaaQualityLevels.NumQualityLevels = 0;
	HRESULT	hResult = m_device->GetDevice()->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &d3dMsaaQualityLevels, sizeof(D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS));
	m_screenInfo.nMsaa4xQualityLevels = d3dMsaaQualityLevels.NumQualityLevels;
	m_screenInfo.bMsaa4xEnable = (m_screenInfo.nMsaa4xQualityLevels > 1) ? true : false;
}
