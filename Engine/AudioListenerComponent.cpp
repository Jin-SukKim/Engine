#include "pch.h"
#include "AudioListenerComponent.h"
#include "Object.h"
#include "Transform2DComponent.h"
#include "TransformComponent.h"
#include "Fmod.h"

namespace JE {
	void AudioListenerComponent::Tick(const float& DeltaTime)
	{
		Object* owner = GetOwner();
		if (!owner)
			return;

		Transform2DComponent* tr2D = owner->GetComponent<Transform2DComponent>();
		if (tr2D) {
			Vector2 pos = tr2D->GetPos();
			Vector2 vel = Vector2::Zero;
			Fmod::Set3DListenerAttributes(pos, vel);
			return;
		}

		TransformComponent* tr = owner->GetComponent<TransformComponent>();
		if (tr) {
			Vector3 pos = tr->GetPos();
			Vector3 vel = Vector3::Zero;
			Fmod::Set3DListenerAttributes(pos, vel);
		}
	}
}