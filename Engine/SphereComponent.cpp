#include "pch.h"
#include "SphereComponent.h"
#include "BoxComponent.h"
#include "AssetManager.h"
#include "Actor.h"
#include "TransformComponent.h"
#include "Renderer/CppRenderer3D.h"
#include "Mesh.h"
#include "Math/Box.h"
#include "Math/Sphere.h"

namespace JE {
	void SphereComponent::Init()
	{
		Super::Init();

		// TODO: Box shape
		//AssetManager::Load<Mesh2D>(L"BoxMesh", MeshData::SquareVertex, MeshData::SquareIndices);

		//_mesh = AssetManager::Find<Mesh>(L"BoxMesh");
		_transform = std::make_unique<TransformComponent>(L"BoundingTransform");
		UpdateSphere();
	}

	void SphereComponent::Tick(const float& DeltaTime)
	{
		Super::Tick(DeltaTime);

		UpdateSphere();
	}

	void SphereComponent::Render(IRenderer* r)
	{
		Super::Render(r);

		IRenderer3D* r3d = dynamic_cast<IRenderer3D*>(r);
		if (r3d == nullptr)
			return;

		r3d->DrawMesh(_mesh, _transform.get(), nullptr);
	}

	bool SphereComponent::CheckCollision(Collider* other)
	{
		if (Super::CheckCollision(other) == false)
			return false;

		switch (other->GetColliderType())
		{
		case ColliderType::Box:
			return CheckCollisionSphereToBox(this->GetSphere(), dynamic_cast<BoxComponent*>(other)->GetBox());
		case ColliderType::Sphere:
			return this->GetSphere().Intersect(dynamic_cast<SphereComponent*>(other)->GetSphere());
		}

		return false;
	}

	void SphereComponent::UpdateSphere()
	{
		Actor* owner = dynamic_cast<Actor*>(this->GetOwner());
		if (!owner)
			return;

		TransformComponent* tr = owner->GetComponent<TransformComponent>();
		if (!tr)
			return;

		// Collider의 위치와 크기에 맞게 Transform 수정
		_transform->SetPos(tr->GetPos() + GetOffset().ToVector2());
		_transform->SetScale(tr->GetScale() * GetScale().ToVector2());

		std::vector<Vertex3D> vertices = _mesh->GetVertices();
		Matrix4x4 mat = _transform->GetTransformMatrix();
		for (Vertex3D& v : vertices) {
			v.Pos = v.Pos * mat;
		}
		
		_bound.UpdateRadius(vertices);
	}
}