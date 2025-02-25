#pragma once
#include "Fmod\fmod_studio.hpp"
#include "Fmod\\fmod.hpp"
#include "Fmod\\fmod_common.h"
#include "Fmod\\fmod_codec.h"

namespace JE {
	class Fmod
	{
	public:
		static void Initialize();
		static bool CreateSound(const std::string path, FMOD::Sound** sound);
		static void SoundPlay(FMOD::Sound* sound, FMOD::Channel** channel);
		static void Set3DListenerAttributes(const Vector3& pos = Vector3::Zero, const Vector3& vel = Vector3::Zero);
		static void Release();
	private:
		static FMOD::Studio::System* System;
		static FMOD::System* CoreSystem;
	};
}
