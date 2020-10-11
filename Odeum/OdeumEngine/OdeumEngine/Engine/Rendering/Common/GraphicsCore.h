#ifndef GRAPHICSCORE_H
#define GRAPHICSCORE_H

#include "../../../pch.h"

#define SWAP_CHAIN_BUFFER_COUNT 3

namespace Graphics
{
	extern uint32_t s_displayWidth;
	extern uint32_t s_displayHeight;

	extern UINT s_currentBuffer;

	uint64_t GetFrameCount();
	float GetFrameTime();
	void SetFrameTime(float frameTime);
	float GetFrameRate();

	enum ResolutionOptions { k720p, k900p, k1080p, k1440p, k1800p, k2160p };

	extern bool s_enableVSync;
	extern ResolutionOptions s_targetResolution;
	extern bool s_ultraWide;
	extern uint32_t s_frameCounter;
}

#endif