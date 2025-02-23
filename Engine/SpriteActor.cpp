#include "pch.h"
#include "SpriteActor.h"
#include "Transform2DComponent.h"

namespace JE {
	SpriteActor::SpriteActor(const std::wstring& name) : Super(name)
	{

	}

	void SpriteActor::Init()
	{
		Super::Init();
		// 모든 2D Actor에 Transform2DComponent 생성
		initTransform();
	}

	void SpriteActor::Tick(const float& DeltaTime)
	{
		Super::Tick(DeltaTime);
	}

	void SpriteActor::Render(IRenderer* r)
	{
		Super::Render(r);
	}

	void SpriteActor::initTransform()
	{
		Transform2DComponent* tr = AddComponent<Transform2DComponent>(L"Transform");
	}
}