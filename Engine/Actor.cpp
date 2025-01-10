#include "pch.h"
#include "Actor.h"
#include "TransformComponent.h"

namespace JE {
	Actor::Actor(const std::wstring& name) : Super(name)
	{
	}

	void Actor::Init()
	{
		Super::Init();
		// 모든 Actor에 TransformComponent 추가
		initTransform();
	}

	void Actor::Tick(const float& DeltaTime)
	{
		Super::Tick(DeltaTime);
	}

	void Actor::Render()
	{
		Super::Render();
	}

	void Actor::initTransform()
	{
		TransformComponent* tr = AddComponent<TransformComponent>(L"Transform");
	}
}