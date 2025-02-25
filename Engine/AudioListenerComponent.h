#pragma once
#include "Component.h"

namespace JE {
    // Sound¸¦ µè´Â Component
    class AudioListenerComponent : public Component
    {
        using Super = Component;
    public:
        AudioListenerComponent() : Super(L"AudioListener", ComponentType::AudioListner) {}
        ~AudioListenerComponent() override {}

        void Init() override {};
        void Tick(const float& DeltaTime) override;
        void Render(IRenderer* r) override {};
    };
}
