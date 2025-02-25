#include "pch.h"
#include "AudioSourceComponent.h"
#include "Object.h"
#include "AudioClip.h"
#include "Transform2DComponent.h"
#include "TransformComponent.h"

namespace JE {
	void AudioSourceComponent::Init()
	{
	}
	void AudioSourceComponent::Tick(const float& DeltaTime)
	{
		Object* owner = GetOwner();
		if (!owner)
			return;

		Transform2DComponent* tr2D = GetOwner()->GetComponent<Transform2DComponent>();
		if (tr2D) {
			Vector2 pos = tr2D->GetPos();
			_audioClip->Set3DAttributes(pos);
			return;
		}

		TransformComponent* tr = GetOwner()->GetComponent<TransformComponent>();
		if (tr) {
			Vector3 pos = tr->GetPos();
			_audioClip->Set3DAttributes(pos);
		}
	}
	void AudioSourceComponent::Render(IRenderer* r)
	{
	}
	void AudioSourceComponent::Play()
	{
		_audioClip->Play();
	}
	void AudioSourceComponent::Stop()
	{
		_audioClip->Stop();
	}
	void AudioSourceComponent::SetLoop(bool loop)
	{
		_audioClip->SetLoop(loop);
	}
}