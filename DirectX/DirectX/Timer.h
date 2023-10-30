#pragma once
class Timer
{
public:
	static Timer* GetInst()
	{
		static Timer* timer = new Timer;
		return timer;
	}
	float GetDeltaTime()
	{
		return m_deltaTime;
	}
	void Update();
	void Release()
	{
		delete this;
	}
	
private:
	Timer();
	LARGE_INTEGER m_currentTime;
	LARGE_INTEGER m_previousTime;
	LARGE_INTEGER m_frequency;
	float m_deltaTime = 0.f;
	float m_curTime = 0.f;
};

