#ifndef AUDIOSOURCE_H
#define AUDIOSOURCE_H

#include "../../pch.h"
#include "../Engine/Core/Component.h"

class AudioSource : public Component
{
public: 
	
	// overridden functions
	void OnAttach(GameObject* parent) override;
	void OnDetach() {};
	void Update(float deltaTime) override;
	void Initialize(std::string soundName, bool isLoop = false, bool is3D = false, bool isLong = false, float volume = 1.0f);

	void PlaySound();
	bool IsPlaying();

private:
	std::string as_audioFile;
	int as_channelID;
	float as_volume;
};
#endif