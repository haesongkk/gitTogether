#include "framework.h"
#include "Timer.h"

void Timer::Update()
{
	m_previousTime = m_currentTime;
	QueryPerformanceCounter(&m_currentTime);
	m_deltaTime = (float)(m_currentTime.QuadPart - m_previousTime.QuadPart) / (float)(m_frequency.QuadPart);
}

Timer::Timer()
{
	QueryPerformanceFrequency(&m_frequency);
	QueryPerformanceCounter(&m_previousTime);
	QueryPerformanceCounter(&m_currentTime);
}
