#include "EngineInfo.h"
#include "Engine.h"
#include "CommandQueue.h"
#include "SwapChain.h"
#include "GameFramework.h"
#include "DepthStencilBuffer.h"
#include "RootSignature.h"

void CCommandQueue::Init(ID3D12Device* device, CSwapChain* swapChain)
{
	m_swapChain = swapChain;

	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc;
	::ZeroMemory(&cmdQueueDesc, sizeof(D3D12_COMMAND_QUEUE_DESC));
	cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	HRESULT hResult = device->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&m_cmdQueue));

	hResult = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_cmdAlloc));

	hResult = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_cmdAlloc, NULL, IID_PPV_ARGS(&m_cmdList));
	hResult = m_cmdList->Close();

	device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence));
	m_fenceEvent = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);
}

void CCommandQueue::OnDestroy()
{
	::CloseHandle(m_fenceEvent);

	if (m_cmdAlloc) m_cmdAlloc->Release();
	if (m_cmdQueue) m_cmdQueue->Release();
	if (m_cmdList) m_cmdList->Release();
	if (m_fence) m_fence->Release();
}

void CCommandQueue::WaitGpuComplete()
{
	UINT64 nFenceValue = ++m_fenceValue;
	HRESULT hResult = m_cmdQueue->Signal(m_fence, nFenceValue);
	if (m_fence->GetCompletedValue() < nFenceValue)
	{
		hResult = m_fence->SetEventOnCompletion(nFenceValue, m_fenceEvent);
		::WaitForSingleObject(m_fenceEvent, INFINITE);
	}
}

void CCommandQueue::RenderBegin(const D3D12_VIEWPORT* vp, const D3D12_RECT* rect)
{
	m_cmdAlloc->Reset();
	m_cmdList->Reset(m_cmdAlloc, nullptr);


	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		m_swapChain->GetBackRTVBuffer(),
		D3D12_RESOURCE_STATE_PRESENT, 
		D3D12_RESOURCE_STATE_RENDER_TARGET);

	m_cmdList->SetGraphicsRootSignature(CEngine::GetInst()->GetFramework()->GetRootSignature()->GetRootSignature());
	m_cmdList->RSSetViewports(1, vp);
	m_cmdList->RSSetScissorRects(1, rect);

	m_cmdList->ResourceBarrier(1, &barrier);

	D3D12_CPU_DESCRIPTOR_HANDLE backBufferView = m_swapChain->GetBackRTV();
	D3D12_CPU_DESCRIPTOR_HANDLE depthStencilView = CEngine::GetInst()->GetFramework()->GetDepthStencilBuffer()->GetDsvCpuHandle();

	m_cmdList->OMSetRenderTargets(1, &backBufferView, FALSE, &depthStencilView);
	m_cmdList->ClearRenderTargetView(backBufferView, Colors::Black, 0, nullptr);
	m_cmdList->ClearDepthStencilView(depthStencilView, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, (UINT8)0.0f, 0, nullptr);
}

void CCommandQueue::RenderEnd()
{
	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		m_swapChain->GetBackRTVBuffer(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, 
		D3D12_RESOURCE_STATE_PRESENT); 

	m_cmdList->ResourceBarrier(1, &barrier);
	m_cmdList->Close();

	ID3D12CommandList* cmdListArr[] = { m_cmdList };
	m_cmdQueue->ExecuteCommandLists(_countof(cmdListArr), cmdListArr);

	m_swapChain->Present();

	WaitGpuComplete();

	m_swapChain->SwapIndex();
}
