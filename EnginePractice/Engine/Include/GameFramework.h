#pragma once

class CGameFramework
{
public:
	CGameFramework(HINSTANCE hInst, HWND hWnd, int windowWidth, int windowHeight);

public:
	class CDepthStencilBuffer* GetDepthStencilBuffer() { return m_depthStencilBuffer; }


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


	class CDevice				* m_device;
	class CCommandQueue			* m_cmdQueue;
	class CSwapChain			*m_swapChain;
	CDepthStencilBuffer	*m_depthStencilBuffer;
};

