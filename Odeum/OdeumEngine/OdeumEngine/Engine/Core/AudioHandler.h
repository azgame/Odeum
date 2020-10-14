#ifndef AUDIOHANDLER_H
#define AUDIOHANDLER_H

#include "../../pch.h"
#include <string>
#include <map>
#include <memory>
#include <fmod.hpp>
#include "../Math/Vector.h"
// might not need this here vv
#include "../Game/Components/AudioSource.h"

#define MAX_AUDIO_CHANNELS 10

class AudioHandler
{
public:
	AudioHandler(const AudioHandler&) = delete;
	AudioHandler(AudioHandler&&) = delete;
	AudioHandler& operator=(const AudioHandler&) = delete;
	AudioHandler& operator=(AudioHandler&&) = delete;

	bool Initialize(Vector3 position = Vector3(), Vector3 velocity = Vector3(), Vector3 forward = Vector3(0.0f, 0.0f, -1.0f), Vector3 up = Vector3(0.0f, 1.0f, 0.0f));
	static AudioHandler* GetInstance();

	void OnDestroy();
	void Update();

private:
	friend class AudioSource;
	AudioHandler();
	~AudioHandler();

	static std::unique_ptr<AudioHandler> audioHandlerInstance;
	friend std::default_delete<AudioHandler>;

	FMOD::System* system;

	static std::map<std::string, FMOD::Sound*> soundMap;
	static std::map<int, FMOD::Channel*> channelMap;

	int numOfChannels;

	// private methods
	FMOD_VECTOR MakeFMODVector(Vector3 vec);
	void LoadSound(std::string soundName, bool isLoop = false, bool is3D = false, bool isLong = false);
	FMOD::Sound* GetSound(std::string soundName);
	int PlaySound(std::string soundName, Vector3 position, Vector3 velocity, float volume);
	void UpdateChannel(int channelNum, Vector3 position, Vector3 velocity);
	bool IsPlaying(int channelNum);

};

#endif

