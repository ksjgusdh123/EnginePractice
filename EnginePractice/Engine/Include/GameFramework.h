#pragma once

struct Vertex
{
	XMFLOAT3 position;
	XMFLOAT4 color;
};

class CGameFramework
{
public:
	CGameFramework(HINSTANCE hInst, HWND hWnd, int windowWidth, int windowHeight);

public:
	class CDepthStencilBuffer* GetDepthStencilBuffer() { return m_depthStencilBuffer; }
	class CDevice* GetDevice() { return m_device; }
	class CRootSignature* GetRootSignature() { return m_rootSignature; }
	class CCommandQueue* GetcmdQueue() { return m_cmdQueue; }
public:
	bool Init();
	void Update();
	void Render();
	void RenderBegin();
	void RenderEnd();


	void CheckMsaa();

private:
	HINSTANCE					m_hInst = 0;
	HWND						m_hWnd = 0;
	D3D12_VIEWPORT				m_viewport;
	D3D12_RECT					m_scissorRect;

	ScreenInfo					m_screenInfo;

	int							m_windowWidth;
	int							m_windowHeight;


	CDevice				* m_device;
	CCommandQueue		*m_cmdQueue;
	class CSwapChain	*m_swapChain;
	CDepthStencilBuffer	*m_depthStencilBuffer;
	CRootSignature		*m_rootSignature;
	class CShader		*m_shader;

	ComPtr<ID3D12Resource> m_vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
};

