#include "AudioSource.h"
#include "../Engine/Core/AudioHandler.h"
#include "Rigidbody.h"

void AudioSource::OnAttach(GameObject* parent)
{
	m_gameObject = parent;
}

void AudioSource::Initialize(std::string soundName, bool isLoop, bool is3D, bool isLong, float volume)
{
	as_channelID = -1;
	as_audioFile = soundName;
	as_volume = volume;
	if (as_volume > 1.0f)
	{
		as_volume = 1.0f;
	}
	else if (as_volume < 0.0f)
	{
		as_volume = 0.0f;
	}
	AudioHandler::GetInstance()->LoadSound(soundName, isLoop, is3D, isLong);
}

void AudioSource::PlaySound()
{
	if (m_gameObject->GetComponent<Rigidbody>()) {
		as_channelID = AudioHandler::GetInstance()->PlaySound(as_audioFile, m_gameObject->GetComponent<Rigidbody>()->GetPosition().GetVec(), m_gameObject->GetComponent<Rigidbody>()->GetVelocity().GetVec(), as_volume);
	} 
	else
	{
		as_channelID = AudioHandler::GetInstance()->PlaySound(as_audioFile, Vector3(), Vector3(), as_volume);
	}
}

bool AudioSource::IsPlaying()
{
	return AudioHandler::GetInstance()->IsPlaying(as_channelID);
}

void AudioSource::Update(float deltaTime)
{

}