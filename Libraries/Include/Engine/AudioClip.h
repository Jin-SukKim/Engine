#pragma once
#include "Resource.h"
#include "Fmod.h"

namespace JE {
    class AudioClip : public Resource
    {
        using Super = Resource;
    public:
        AudioClip() : Super(L"AudioClip", ResourceType::AudioClip) {}
        virtual ~AudioClip() override;

        virtual bool Load(const std::wstring& path) override;

        void Play();
        void Stop();
        void Set3DAttributes(const Vector3& pos = Vector3::Zero, const Vector3& vel = Vector3::Zero);
        void SetLoop(bool loop) { _bLoop = loop; }
    private:
        FMOD::Sound* _sound = nullptr;
        FMOD::Channel* _channel = nullptr;
        // 어느정도 거리에서 들리는지
        float _minDistance = 1.f;
        float _maxDistance = 1000.f;
        bool _bLoop = false;
    };
}
