#include "EngineInfo.h"
#include "GameFramework.h"
#include "SwapChain.h"
#include "CommandQueue.h"
#include "DepthStencilBuffer.h"
#include "RootSignature.h"
#include "Shader.h"
#include "ConstantBuffer.h"

CGameFramework::CGameFramework(HINSTANCE hInst, HWND hWnd, int windowWidth, int windowHeight)
	: m_hInst(hInst), m_hWnd(hWnd)
{
	m_screenInfo.width = windowWidth;
	m_screenInfo.height = windowHeight;
	m_screenInfo.hWnd = hWnd;

	m_swapChain = new CSwapChain();
	m_cmdQueue = new CCommandQueue();
	m_depthStencilBuffer = new CDepthStencilBuffer();
	m_rootSignature = new CRootSignature();
	m_shader = new CShader();
}

CGameFramework::~CGameFramework()
{
}

CConstantBuffer* CGameFramework::GetConstantBuffer(CONSTANT_BUFFER_TYPE type)
{
	return m_constantBuffers[static_cast<UINT8>(type)];
}

bool CGameFramework::Init()
{
	m_viewport = { 0, 0, static_cast<FLOAT>(m_screenInfo.width), static_cast<FLOAT>(m_screenInfo.height), 0.0f, 1.0f };
	m_scissorRect = CD3DX12_RECT(0, 0, m_screenInfo.width, m_screenInfo.height);

	InitDevice();
	m_cmdQueue->Init(m_device, m_swapChain);
	CheckMsaa();
	m_swapChain->Init(m_device, m_factory, m_cmdQueue->GetCmdQueue(), m_screenInfo);
	m_depthStencilBuffer->Init(m_device, m_screenInfo);
	m_rootSignature->Init();

	ShaderInfo info =
	{
		RASTERIZER_TYPE::CULL_NONE,
		DEPTH_STENCIL_TYPE::LESS_EQUAL
	};
	m_shader->Init(L"..\\..\\Engine\\Include\\shader.hlsl", info);
	CreateConstantBuffer(sizeof(XMFLOAT3), 1);

	return true;
}

void CGameFramework::Update()
{
	Render();
}

void CGameFramework::Render()
{
	RenderBegin();

	
	RenderEnd();
}

void CGameFramework::RenderBegin()
{
	m_cmdQueue->RenderBegin(&m_viewport, &m_scissorRect);
	// temp
	XMFLOAT3 pos(0.5, 0.5, 0.5);
	GetConstantBuffer(CONSTANT_BUFFER_TYPE::GLOBAL)->PushConstantBufferViewData(m_cmdQueue->GetCmdList(), &pos, sizeof(pos));
	m_shader->Update();
}

void CGameFramework::RenderEnd()
{
	m_cmdQueue->RenderEnd();
}

void CGameFramework::OnDestroy()
{
	for (int i = 0; i < 1; ++i)
	{
		m_constantBuffers[i]->tempDestroy();
		delete m_constantBuffers[i];
	}
	m_depthStencilBuffer->OnDestroy();
	m_swapChain->OnDestroy();
	m_cmdQueue->OnDestroy();
	m_shader->tempDestroy();
	m_rootSignature->OnDestroy();

	if (m_device) m_device->Release();
	if (m_factory) m_factory->Release();

	delete m_depthStencilBuffer;
	delete m_swapChain;
	delete m_cmdQueue;
	delete m_shader;
	delete m_rootSignature;
}

void CGameFramework::InitDevice()
{
	HRESULT hResult;

	UINT nDXGIFactoryFlags = 0;
#if defined(_DEBUG)
	ID3D12Debug* debugController = NULL;
	hResult = D3D12GetDebugInterface(__uuidof(ID3D12Debug), (void**)&debugController);
	if (debugController)
	{
		debugController->EnableDebugLayer();
		debugController->Release();
	}
	nDXGIFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif

	hResult = CreateDXGIFactory2(nDXGIFactoryFlags, IID_PPV_ARGS(&m_factory));

	IDXGIAdapter1* adapter = NULL;

	for (UINT i = 0; DXGI_ERROR_NOT_FOUND != m_factory->EnumAdapters1(i, &adapter); i++)
	{
		DXGI_ADAPTER_DESC1 dxgiAdapterDesc;
		adapter->GetDesc1(&dxgiAdapterDesc);
		if (dxgiAdapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) continue;
		if (SUCCEEDED(D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&m_device)))) break;
	}

	if (!m_device)
	{
		m_factory->EnumWarpAdapter(IID_PPV_ARGS(&adapter));
		hResult = D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&m_device));
	}

	if (adapter) adapter->Release();
}

void CGameFramework::CheckMsaa()
{
	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS d3dMsaaQualityLevels;
	d3dMsaaQualityLevels.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dMsaaQualityLevels.SampleCount = 4;
	d3dMsaaQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	d3dMsaaQualityLevels.NumQualityLevels = 0;
	HRESULT	hResult = m_device->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &d3dMsaaQualityLevels, sizeof(D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS));
	m_screenInfo.nMsaa4xQualityLevels = d3dMsaaQualityLevels.NumQualityLevels;
	m_screenInfo.bMsaa4xEnable = (m_screenInfo.nMsaa4xQualityLevels > 1) ? true : false;
}

void CGameFramework::CreateConstantBuffer(UINT32 bufferSize, UINT32 count)
{
	CConstantBuffer* buffer = new CConstantBuffer();
	buffer->Init(m_device, bufferSize, count);
	m_constantBuffers.push_back(buffer);
}
