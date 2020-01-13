#ifndef TIMER_H
#define TIMER_H

#include "../../pch.h"

class Timer
{
public:
	Timer(const Timer&) = delete;
	Timer(Timer&&) = delete;
	Timer& operator=(const Timer&) = delete;
	Timer& operator=(Timer&&) = delete;

	Timer();
	void Initialize();
	void UpdateFrameTicks();
	float GetDeltaTime() const;
	unsigned int GetSleepTime(const unsigned int fps_);
	float GetCurrentTicks() const;

private:
	unsigned int m_prevTicks, m_currentTicks;
};

#endif