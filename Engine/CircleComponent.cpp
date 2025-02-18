#include "pch.h"
#include "CircleComponent.h"
#include "RectangleComponent.h"
#include "AssetManager.h"
#include "Mesh2DComponent.h"
#include "SpriteActor.h"
#include "Transform2DComponent.h"
#include "Renderer/CppRenderer2D.h"

namespace JE {
	void CircleComponent::Init()
	{
		Super::Init();

		// TODO: Circle로 생성하기
		//AssetManager::Load<Mesh2D>(L"CircleMesh", MeshData::CircleVertex, MeshData::CircleIndices);

		_mesh = std::make_unique<Mesh2DComponent>(L"CircleColliderMesh");
		//_mesh->SetMesh(L"CircleMesh");
	}
	void CircleComponent::Render(IRenderer* r)
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

	bool CircleComponent::CheckCollision(Collider* other)
	{
		if (Super::CheckCollision(other) == false) // 서로 충돌할 수 있는지 확인
			return false;

		switch (other->GetColliderType())
		{
		case ColliderType::Rectangle:
			return CheckCollisionCircleToRect(this, dynamic_cast<RectangleComponent*>(other));
		case ColliderType::Circle:
			return CheckCollisionCircleToCircle(this, dynamic_cast<CircleComponent*>(other));
		}

		return false;
	}

	Vector2 CircleComponent::GetPos() const
	{
		Vector2 offset = GetOffset().ToVector2();
		SpriteActor* owner = dynamic_cast<SpriteActor*>(this->GetOwner());
		if (!owner)
			return offset;

		Transform2DComponent* tr = owner->GetComponent<Transform2DComponent>();
		if (!tr)
			return offset;

		return tr->GetPos() + offset;
	}
}