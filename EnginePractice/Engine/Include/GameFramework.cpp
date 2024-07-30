#include "EngineInfo.h"
#include "GameFramework.h"
#include "Device.h"
#include "SwapChain.h"
#include "CommandQueue.h"
#include "DepthStencilBuffer.h"

CGameFramework::CGameFramework(HINSTANCE hInst, HWND hWnd, int windowWidth, int windowHeight)
	: m_hInst(hInst), m_hWnd(hWnd)
{
	screenInfo.width = windowWidth;
	screenInfo.height = windowHeight;

	m_device = new CDevice();
	m_swapChain = new CSwapChain();
	m_cmdQueue = new CCommandQueue();
	m_depthStencilBuffer = new CDepthStencilBuffer();
}

bool CGameFramework::Init()
{
	
	m_device->Init();
	m_cmdQueue->Init(m_device->GetDevice(), m_swapChain);
	CheckMsaa();
	m_swapChain->Init();
	m_depthStencilBuffer->Init(m_device->GetDevice(), screenInfo);
	return true;
}

void CGameFramework::CheckMsaa()
{
	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS d3dMsaaQualityLevels;
	d3dMsaaQualityLevels.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dMsaaQualityLevels.SampleCount = 4;
	d3dMsaaQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	d3dMsaaQualityLevels.NumQualityLevels = 0;
	HRESULT	hResult = m_device->GetDevice()->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &d3dMsaaQualityLevels, sizeof(D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS));
	screenInfo.nMsaa4xQualityLevels = d3dMsaaQualityLevels.NumQualityLevels;
	screenInfo.bMsaa4xEnable = (screenInfo.nMsaa4xQualityLevels > 1) ? true : false;
}
