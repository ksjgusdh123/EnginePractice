#pragma once
class CTimer
{
public:
	UINT32 GetFps() { return m_fps; }
	float GetDeltaTime() { return m_deltaTime; }

public:
	void Init();
	void Update();

private:
	UINT64 m_frequency = 0;
	UINT64 m_prevCount = 0;
	float m_deltaTime = 0.f;

private:
	UINT32 m_frameCount = 0;
	float m_frameTime = 0.f;
	UINT32 m_fps = 0;
};

