#include "EngineInfo.h"
#include "SwapChain.h"

void CSwapChain::Init(ID3D12Device* device, IDXGIFactory* factory, ID3D12CommandQueue* cmdQueue, ScreenInfo& info, bool windowed)
{
	CreateSwapChain(factory, cmdQueue, info, windowed);
	CreateRTV(device);
}

void CSwapChain::OnDestroy()
{
	for (int i = 0; i < SWAP_CHAIN_BUFFER_COUNT; ++i)
	{
		if (m_rtvBuffer[i]) m_rtvBuffer[i]->Release();
	}

	if (m_rtvHeap) m_rtvHeap->Release();

	m_swapChain->SetFullscreenState(FALSE, NULL);
	if (m_swapChain) m_swapChain->Release();
}

void CSwapChain::CreateSwapChain(IDXGIFactory* factory, ID3D12CommandQueue* cmdQueue, ScreenInfo& info, bool windowed)
{
	DXGI_SWAP_CHAIN_DESC sd;
	::ZeroMemory(&sd, sizeof(sd));
	sd.BufferDesc.Width = static_cast<UINT32>(info.width); // 버퍼의 해상도 너비
	sd.BufferDesc.Height = static_cast<UINT32>(info.height); // 버퍼의 해상도 높이
	sd.BufferDesc.RefreshRate.Numerator = 60; // 화면 갱신 비율
	sd.BufferDesc.RefreshRate.Denominator = 1; // 화면 갱신 비율
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 버퍼의 디스플레이 형식
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = 1; // 멀티 샘플링 OFF
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // 후면 버퍼에 렌더링할 것 
	sd.BufferCount = SWAP_CHAIN_BUFFER_COUNT; // 전면+후면 버퍼
	sd.OutputWindow = info.hWnd;
	sd.Windowed = windowed;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // 전면 후면 버퍼 교체 시 이전 프레임 정보 버림
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	factory->CreateSwapChain(cmdQueue, &sd, (IDXGISwapChain**) & m_swapChain);

	for (INT32 i = 0; i < SWAP_CHAIN_BUFFER_COUNT; i++)
		m_swapChain->GetBuffer(i, IID_PPV_ARGS(&m_rtvBuffer[i]));
}

void CSwapChain::CreateRTV(ID3D12Device* device)
{
	INT32 rtvHeapSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	D3D12_DESCRIPTOR_HEAP_DESC d3dDescriptorHeapDesc;
	::ZeroMemory(&d3dDescriptorHeapDesc, sizeof(D3D12_DESCRIPTOR_HEAP_DESC));
	d3dDescriptorHeapDesc.NumDescriptors = SWAP_CHAIN_BUFFER_COUNT;
	d3dDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	d3dDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	d3dDescriptorHeapDesc.NodeMask = 0;

	HRESULT hResult = device->CreateDescriptorHeap(&d3dDescriptorHeapDesc, IID_PPV_ARGS(&m_rtvHeap));

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHeapBegin = m_rtvHeap->GetCPUDescriptorHandleForHeapStart();

	for (int i = 0; i < SWAP_CHAIN_BUFFER_COUNT; i++)
	{
		m_rtvHandle[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeapBegin, i * rtvHeapSize);
		device->CreateRenderTargetView(m_rtvBuffer[i],  nullptr, m_rtvHandle[i]);
	}
}

void CSwapChain::Present()
{
	m_swapChain->Present(0, 0);
}

void CSwapChain::SwapIndex()
{
	m_backBufferIndex = (m_backBufferIndex + 1) % SWAP_CHAIN_BUFFER_COUNT;
}
