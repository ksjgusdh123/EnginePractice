#pragma once

#include "EngineInfo.h"

class CEngine
{
public:
	bool Init(HINSTANCE hInst, int windowWidth, int windowHeight);
	int Run();

public:
	class CGameFramework* GetFramework();

public:
	void Logic();
	bool Update(float elapsedTime);
	bool PostUpdate(float elapsedTime);
	void Render(float elapsedTime);

private:
	void Register();	// â Ŭ���� ���
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

