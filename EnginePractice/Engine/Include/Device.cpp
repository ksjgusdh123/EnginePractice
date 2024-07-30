#include "EngineInfo.h"
#include "Device.h"

void CDevice::Init()
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


}
