#include "pch.h"
#include "RectangleComponent.h"
#include "CircleComponent.h"
#include "AssetManager.h"
#include "SpriteActor.h"
#include "Transform2DComponent.h"
#include "Renderer/CppRenderer2D.h"

namespace JE {
	void RectangleComponent::Init()
	{
		Super::Init();

		AssetManager::Load<Mesh2D>(L"SquareMesh", MeshData::SquareVertex, MeshData::SquareIndices);

		_mesh = std::make_unique<Mesh2DComponent>(L"RectangleColliderMesh");
		_mesh->SetMesh(L"SquareMesh");
	}

	void RectangleComponent::Render(IRenderer* r)
	{
		Super::Render(r);

		IRenderer2D* r2d = dynamic_cast<IRenderer2D*>(r);
		if (r2d == nullptr)
			return;

		SpriteActor* owner = dynamic_cast<SpriteActor*>(this->GetOwner());
		if (!owner)
			return;

		Transform2DComponent* tr = owner->GetComponent<Transform2DComponent>();
		if (!tr)
			return;

		// Collider의 위치와 크기에 맞게 Transform 수정
		tr->AddPos(GetOffset().ToVector2());
		tr->SetScale(GetSize().ToVector2());

		r2d->DrawMesh(_mesh->GetMesh(), tr, nullptr);
	}

	bool RectangleComponent::CheckCollision(Collider* other)
	{
		if (Super::CheckCollision(other) == false)
			return false;

		switch (other->GetColliderType())
		{
		case ColliderType::Rectangle:
			return CheckCollisionRectToRect(this, dynamic_cast<RectangleComponent*>(other));
		case ColliderType::Circle:	
			return CheckCollisionCircleToRect(dynamic_cast<CircleComponent*>(other), this);
		}

		return false;
	}

	RECT RectangleComponent::GetRect() const
	{
		SpriteActor* owner = dynamic_cast<SpriteActor*>(this->GetOwner());
		if (!owner)
			return RECT(0, 0, 0, 0);

		Transform2DComponent* tr = owner->GetComponent<Transform2DComponent>();
		if (!tr)
			return RECT(0, 0, 0, 0);

		Vector2 pos = tr->GetPos() + GetOffset().ToVector2();
		Vector2 halfSize = GetSize().ToVector2() * 0.5f;

		RECT rect = {
			static_cast<int32>(pos.X - halfSize.X),
			static_cast<int32>(pos.Y - halfSize.Y),
			static_cast<int32>(pos.X + halfSize.X),
			static_cast<int32>(pos.Y + halfSize.Y)
		};

		return rect;
	}
}