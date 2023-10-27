#include "framework.h"
#include "Timer.h"

void Timer::Update()
{
	auto preTime = m_curTime;
	m_curTime = GetTickCount64() / 1000.f;
	m_deltatime = m_curTime - preTime;
}

Timer::Timer()
	:m_curTime(GetTickCount64() / 1000.f) { }
