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
		// ��� Actor�� TransformComponent �߰�
		initTransform();
	}

	void Actor::Tick(const float& DeltaTime)
	{
		Super::Tick(DeltaTime);
	}

	void Actor::Render(IRenderer* r)
	{
		Super::Render(r);
	}

	void Actor::initTransform()
	{
		TransformComponent* tr = AddComponent<TransformComponent>(L"Transform");
	}
}