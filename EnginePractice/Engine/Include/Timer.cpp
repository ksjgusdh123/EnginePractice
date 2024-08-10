#include "EngineInfo.h"
#include "Timer.h"

void CTimer::Init()
{
	::QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&m_frequency));
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&m_prevCount));
}

void CTimer::Update()
{
	UINT64 currentCount;
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currentCount));

	m_deltaTime = (currentCount - m_prevCount) / static_cast<float>(m_frequency);
	m_prevCount = currentCount;

	m_frameCount++;
	m_frameTime += m_deltaTime;

	if (m_frameTime > 1.f)
	{
		m_fps = static_cast<UINT32>(m_frameCount / m_frameTime);

		m_frameTime = 0.f;
		m_frameCount = 0;
	}
}
