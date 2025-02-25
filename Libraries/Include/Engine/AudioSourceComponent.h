#pragma once
#include "Component.h"

namespace JE {
    class AudioClip;
    
    // Sound¸¦ ³»´Â Component
    class AudioSourceComponent : public Component
    {
        using Super = Component;
    public:
        AudioSourceComponent() : Super(L"AudioSource", ComponentType::AudioSource) {}
        virtual ~AudioSourceComponent() override {}

        virtual void Init() override;
        virtual void Tick(const float& DeltaTime) override;
        virtual void Render(IRenderer* r) override;

        void Play();
        void Stop();
        void SetLoop(bool loop);

        void SetClip(AudioClip* clip) { _audioClip = clip; }
        AudioClip* GetClip() { return _audioClip; }
    private:
        AudioClip* _audioClip = nullptr;
    };
}
