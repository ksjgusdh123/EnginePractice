#pragma once

#include "EngineInfo.h"

class CEngine
{
public:
	bool Init(HINSTANCE hInst, int windowWidth, int windowHeight);
	int Run();

public:
	class CGameFramework* GetFramework();

private:
	void Register();	// 창 클래스 등록
	bool Create();

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	CGameFramework* m_gameFramework;

	HINSTANCE	m_hInst = 0;
	HWND		m_hWnd = 0;

	Resolution	m_resolution;

	static bool	m_loop;

	DECLARE_SINGLE(CEngine)
};

