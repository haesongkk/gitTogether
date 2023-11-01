#pragma once
class Timer
{
public:
	static unique_ptr<Timer>& GetInst()
	{
		static unique_ptr<Timer> timer = make_unique<Timer>();
		return timer;
	}
	float GetDeltaTime()
	{
		return m_deltaTime;
	}
	void Update();
	
	Timer();
private:
	LARGE_INTEGER m_currentTime;
	LARGE_INTEGER m_previousTime;
	LARGE_INTEGER m_frequency;
	float m_deltaTime = 0.f;
	float m_curTime = 0.f;
};

