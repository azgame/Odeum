#include "GraphicsCore.h"

namespace Graphics
{
	uint32_t s_displayWidth = 0;
	uint32_t s_displayHeight = 0;

	UINT s_currentBuffer = 0;

	bool s_enableVSync = false;
	ResolutionOptions s_targetResolution = k1080p;
	bool s_ultraWide = false;

	float s_frameTime = 0.0f;
	uint64_t s_frameIndex = 0;
}

uint64_t Graphics::GetFrameCount()
{
	return s_frameIndex;
}

float Graphics::GetFrameTime()
{
	return s_frameTime;
}

void Graphics::SetFrameTime(float frameTime)
{
	s_frameTime = frameTime;
}



float Graphics::GetFrameRate()
{
	return s_frameTime == 0.0f ? 0.0f : 1.0f / s_frameTime;
}