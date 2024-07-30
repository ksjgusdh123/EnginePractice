#pragma once

class CGameFramework
{
public:
	CGameFramework(HINSTANCE hInst, HWND hWnd, int windowWidth, int windowHeight);

public:
	bool Init();

	void CheckMsaa();

private:
	HINSTANCE					m_hInst = 0;
	HWND						m_hWnd = 0;

	ScreenInfo					screenInfo;

	int							m_windowWidth;
	int							m_windowHeight;


	class CDevice				* m_device;
	class CCommandQueue			* m_cmdQueue;
	class CSwapChain			*m_swapChain;
	class CDepthStencilBuffer	*m_depthStencilBuffer;
};

