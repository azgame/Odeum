#include "Timer.h"

Timer::Timer() : m_prevTicks(0), m_currentTicks(0)
{
}

void Timer::Initialize()
{
	m_prevTicks = GetTickCount();
	m_currentTicks = GetTickCount();
}

void Timer::UpdateFrameTicks()
{
	m_prevTicks = m_currentTicks;
	m_currentTicks = GetTickCount();
}

float Timer::GetDeltaTime() const
{
	return static_cast<float>((m_currentTicks - m_prevTicks) / 1000.0f);
}

unsigned int Timer::GetSleepTime(const unsigned int fps_)
{
	unsigned int milliPerFrame = 1000 / fps_;
	if (milliPerFrame == 0)
		return 0;

	unsigned int sleepTime = milliPerFrame - GetTickCount();
	if (sleepTime > milliPerFrame)
		return milliPerFrame;

	return sleepTime;
}

float Timer::GetCurrentTicks() const
{
	return static_cast<float>(m_currentTicks);
}
