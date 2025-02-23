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

		//_mesh = AssetManager::Find<Mesh2D>(L"CircleMesh");
		UpdateCircle();
	}
	void CircleComponent::Tick(const float& DeltaTime)
	{
		Super::Tick(DeltaTime);

		UpdateCircle();
	}
	void CircleComponent::Render(IRenderer* r)
	{
		Super::Render(r);

		IRenderer2D* r2d = dynamic_cast<IRenderer2D*>(r);
		if (r2d == nullptr)
			return;

		r2d->DrawMesh(_mesh, _transform.get(), nullptr);
	}

	bool CircleComponent::CheckCollision(Collider* other)
	{
		if (Super::CheckCollision(other) == false) // 서로 충돌할 수 있는지 확인
			return false;

		switch (other->GetColliderType())
		{
		case ColliderType::Rectangle:
			return CheckCollisionCircleToRect(this->GetCircle(), dynamic_cast<RectangleComponent*>(other)->GetRect());
		case ColliderType::Circle:
			return this->GetCircle().Intersect(dynamic_cast<CircleComponent*>(other)->GetCircle());
		}

		return false;
	}

	void CircleComponent::UpdateCircle()
	{
		SpriteActor* owner = dynamic_cast<SpriteActor*>(this->GetOwner());
		if (!owner)
			return;

		Transform2DComponent* tr = owner->GetComponent<Transform2DComponent>();
		if (!tr)
			return;

		// Collider의 위치와 크기에 맞게 Transform 수정
		_transform->SetPos(tr->GetPos() + GetOffset().ToVector2());
		_transform->SetScale(tr->GetScale() * GetScale().ToVector2());

		std::vector<Vertex2D> vertices = _mesh->GetVertices();
		Matrix3x3 mat = _transform->GetTransformMatrix();
		for (Vertex2D& v : vertices) {
			v.Pos = v.Pos * mat;
		}

		_bound.UpdateRadius(vertices);
	}
}