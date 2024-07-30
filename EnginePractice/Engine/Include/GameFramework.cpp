#include "EngineInfo.h"
#include "GameFramework.h"
#include "Device.h"
#include "SwapChain.h"
#include "CommandQueue.h"
#include "DepthStencilBuffer.h"

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
