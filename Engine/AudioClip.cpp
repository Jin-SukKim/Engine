#include "pch.h"
#include "AudioClip.h"

namespace JE {
	AudioClip::~AudioClip()
	{
		_sound->release();
		_sound = nullptr;
	}

	bool AudioClip::Load(const std::wstring& path)
	{
		std::string cPath(path.begin(), path.end());
		if (!Fmod::CreateSound(cPath, &_sound))
			return false;

		_sound->set3DMinMaxDistance(_minDistance, _maxDistance);
		return true;
	}

	void AudioClip::Play()
	{
		if (_bLoop)
			_sound->setMode(FMOD_LOOP_NORMAL);
		else
			_sound->setMode(FMOD_LOOP_OFF);

		Fmod::SoundPlay(_sound, &_channel);
	}

	void AudioClip::Stop()
	{
		_channel->stop();
	}

	void AudioClip::Set3DAttributes(const Vector3& pos, const Vector3& vel)
	{
		FMOD_VECTOR fmodPos(pos.X, pos.Y, pos.Z);
		FMOD_VECTOR fmodVel(vel.X, vel.Y, vel.Z);

		_channel->set3DAttributes(&fmodPos, &fmodVel);
	}
}