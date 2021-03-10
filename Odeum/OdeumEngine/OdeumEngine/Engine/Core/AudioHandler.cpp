#include "AudioHandler.h"

std::unique_ptr<AudioHandler> AudioHandler::audioHandlerInstance = nullptr;
std::map<std::string, FMOD::Sound*> AudioHandler::soundMap;
std::map<int, FMOD::Channel*> AudioHandler::channelMap;

// 
bool AudioHandler::Initialize(Vector3 position, Vector3 velocity, Vector3 forward, Vector3 up)
{
	// create the fmod system
	FMOD::System_Create(&system);

	// get number of drivers
	if (system->getNumDrivers(&numOfChannels) != FMOD_OK) 
	{
		Debug::Error("Error getting number of audio drivers.", "AudioHandler.cpp", __LINE__);
		return false;
	}
	
	// initialize fmod system - 3D by default, also DirectX is also left-handed woo!
	if (system->init(MAX_AUDIO_CHANNELS, FMOD_INIT_NORMAL, nullptr) != FMOD_OK)
	{
		Debug::Error("Error initializing sound system.", "AudioHandler.cpp", __LINE__);
		return false;
	}

	// set listener attributes
	if (system->set3DListenerAttributes(0, &MakeFMODVector(position), &MakeFMODVector(velocity), &MakeFMODVector(forward), &MakeFMODVector(up)) != FMOD_OK)
	{
		Debug::Error("Error setting 3D Listener Attributes.", "AudioHandler.cpp", __LINE__);
		return false;
	}

	return true;
}

// properly destroy the maps and system
void AudioHandler::OnDestroy()
{
	for (auto s : soundMap)
	{
		s.second->release();
	}
	soundMap.clear();

	for (auto c : channelMap)
	{
		c.second->stop();
	}
	channelMap.clear();

	system->release();
	system = nullptr;
}

// necessary for some operating systems
void AudioHandler::Update()
{
	system->update();
}

// if there is an instance of AudioHandler return it, if not create one 
AudioHandler* AudioHandler::GetInstance() 
{
	if (audioHandlerInstance.get() == nullptr) {
		audioHandlerInstance.reset(new AudioHandler);
	}
	return audioHandlerInstance.get();
}

// convert Vector3 to FMOD_VECTOR
FMOD_VECTOR AudioHandler::MakeFMODVector(Vector3 vec)
{
	FMOD_VECTOR fmodVec;
	fmodVec.x = vec.GetX();
	fmodVec.y = vec.GetY();
	fmodVec.z = vec.GetZ();
	return fmodVec;
}

FMOD::Sound* AudioHandler::GetSound(std::string soundName)
{
	// loop through the sound map and check to see if the given sound exists
	// if so, return it. if not, return nullptr
	for (auto s : soundMap)
	{
		if (s.first._Equal(soundName))
		{
			return s.second;
		}
	}
	Debug::Info(soundName + " was not found.", "AudioHandler.cpp", __LINE__);
	return nullptr;
}

bool AudioHandler::LoadSound(std::string soundName, bool isLoop, bool is3D, bool isLong)
{
	// check to see if the sound already exists
	if (GetSound(soundName) != nullptr)
	{
		Debug::Info(soundName + " already exists.", "AudioHandler.cpp", __LINE__);
		return true;
	}

	FMOD_MODE mode;
	mode = FMOD_DEFAULT;

	// add flags using bitwise or 
	if (isLoop) 
	{
		mode |= FMOD_LOOP_NORMAL;
	}
	else
	{
		mode |= FMOD_LOOP_OFF;
	}

	if (is3D)
	{
		mode |= FMOD_3D;
	}
	else
	{
		mode |= FMOD_2D;
	}

	if (isLong)
	{
		mode |= FMOD_CREATESTREAM;
	}
	else
	{
		mode |= FMOD_CREATECOMPRESSEDSAMPLE;
	}

	// create fmod sound
	FMOD::Sound* sound;
	sound = nullptr;

	// if the sound was created successfully add it to the soundMap
	std::string s = "Engine/Resources/Audio/" + soundName;
	FMOD_RESULT r;
	//r = system->createSound(s.c_str(), mode, nullptr, &sound);
	// ^^ r = FMOD_ERR_FILE_NOTFOUND

	if (system->createSound(s.c_str(), mode, nullptr, &sound) != FMOD_OK)
	{
		//sound->release();
		Debug::Error("Error creating the sound", "AudioHandler.cpp", __LINE__);
		return false;
	}
	
	// add to map
	soundMap.insert({ soundName, sound });
	return true;
}

// don't think these defaults are really necessary as all audio will be played through an AudioSource->PlaySound()
int AudioHandler::PlaySound(std::string soundName, Vector3 position, Vector3 velocity = Vector3(), float volume = 1.0f)
{
	// check to see if the sound exists, if not create the sound with default parameters
	int channelID = -1;
	if (GetSound(soundName) == nullptr) {
		Debug::Info(soundName + " was not found. Creating the sound with default parameters now.", "AudioHandler.cpp", __LINE__);
		if (!LoadSound(soundName))
		{
			// end the function -- this memory errors when trying to play from the soundMap when it doesn't have the soundName in there
			return -1;
		}
	}

	// create a channel for the audio
	FMOD::Channel* channel;
	channel = nullptr;

	if (system->playSound(soundMap.at(soundName), nullptr, true, &channel) != FMOD_OK)
	{
		Debug::Error(soundName + " could not be played", "AudioHandler.cpp", __LINE__);

	}

	/*FMOD_MODE* mode;
	mode = new FMOD_MODE(FMOD_3D);
	// I feel like there is a better way for me to do this without having to create a mode pointer
	if (soundMap.at(soundName)->getMode(mode) == FMOD_OK)
	{
		// if the channel was not created successfully
		if (channel->set3DAttributes(&MakeFMODVector(position), &MakeFMODVector(velocity)) != FMOD_OK)
		{
			channel->stop();
			Debug::Error("Channel was not created successfully", "AudioHandler.cpp", __LINE__);
			return -1;
		}
	} 
	
	else {
		Debug::Info(soundName + " was not found. Creating the sound with default parameters now.", "AudioHandler.cpp", __LINE__);
	}
	*/

	// set volume of the channel
	if (channel->setVolume(volume) != FMOD_OK)
	{
		Debug::Error("Error while setting the volume of the channel", "AudioHandler.cpp", __LINE__);
		return -1;
	}


	// play the sound
	if (channel->setPaused(false) != FMOD_OK)
	{
		Debug::Error("Error while unpausing the channel", "AudioHandler.cpp", __LINE__);
		return -1;
	}

	// add channel to map
	channelID = numOfChannels;
	numOfChannels++;

	channelMap.insert({ channelID, channel });
	return channelID;
}

// update 3D attributes of specific channel
void AudioHandler::UpdateChannel(int channelNum, Vector3 position = Vector3(), Vector3 velocity = Vector3())
{
	for (auto c : channelMap)
	{
		if (c.first == channelNum)
		{
			c.second->set3DAttributes(&MakeFMODVector(position), &MakeFMODVector(velocity));
		}
	}
	Debug::Info("Channel " + channelNum + (std::string)" doesn't exist.", "AudioHandler.cpp", __LINE__);
}

// check to see if specific channel is playing
bool AudioHandler::IsPlaying(int channelNum)
{
	for (auto c : channelMap)
	{
		if (c.first == channelNum)
		{
			bool isPlaying;
			c.second->isPlaying(&isPlaying);
			return isPlaying;
		}
	}
	Debug::Info("Channel " + channelNum + (std::string)" doesn't exist.", "AudioHandler.cpp", __LINE__);
	return false;
}