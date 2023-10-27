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
		return m_deltatime;
	}
	void Update();
	
private:
	Timer();
	
	float m_deltatime = 0.f;
	float m_curTime = 0.f;
};

