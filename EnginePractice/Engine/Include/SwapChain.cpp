#include "EngineInfo.h"
#include "SwapChain.h"

void CSwapChain::Init(ID3D12Device* device, IDXGIFactory* factory, ID3D12CommandQueue* cmdQueue, ScreenInfo& info, bool windowed)
{
	CreateSwapChain(factory, cmdQueue, info, windowed);
	CreateRTV(device);
}

void CSwapChain::CreateSwapChain(IDXGIFactory* factory, ID3D12CommandQueue* cmdQueue, ScreenInfo& info, bool windowed)
{
	DXGI_SWAP_CHAIN_DESC sd;
	::ZeroMemory(&sd, sizeof(sd));
	sd.BufferDesc.Width = static_cast<UINT32>(info.width); // ������ �ػ� �ʺ�
	sd.BufferDesc.Height = static_cast<UINT32>(info.height); // ������ �ػ� ����
	sd.BufferDesc.RefreshRate.Numerator = 60; // ȭ�� ���� ����
	sd.BufferDesc.RefreshRate.Denominator = 1; // ȭ�� ���� ����
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // ������ ���÷��� ����
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = 1; // ��Ƽ ���ø� OFF
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // �ĸ� ���ۿ� �������� �� 
	sd.BufferCount = SWAP_CHAIN_BUFFER_COUNT; // ����+�ĸ� ����
	sd.OutputWindow = info.hWnd;
	sd.Windowed = windowed;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // ���� �ĸ� ���� ��ü �� ���� ������ ���� ����
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
