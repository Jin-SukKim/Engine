#include "pch.h"
#include "Actor.h"

namespace JE {
	Actor::Actor(const std::wstring& name) : Super(name)
	{
		// 모든 Actor에 TransformComponent 추가
		initTransform();
	}

	void Actor::Init()
	{
	}

	void Actor::Tick(const float& DeltaTime)
	{
	}

	void Actor::Render()
	{
	}

	void Actor::initTransform()
	{
	}
}