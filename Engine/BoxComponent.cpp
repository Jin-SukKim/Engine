#include "pch.h"
#include "BoxComponent.h"
#include "SphereComponent.h"
#include "AssetManager.h"
#include "Actor.h"
#include "TransformComponent.h"
#include "Renderer/CppRenderer3D.h"
#include "Mesh.h"
#include "Math/Box.h"
#include "Math/Sphere.h"

namespace JE {
	void BoxComponent::Init()
	{
		Super::Init();

		// TODO: Box shape
		//AssetManager::Load<Mesh2D>(L"BoxMesh", MeshData::SquareVertex, MeshData::SquareIndices);

		//_mesh = AssetManager::Find<Mesh>(L"BoxMesh");
		_transform = std::make_unique<TransformComponent>(L"BoundingTransform");
		_bound = Box(_mesh->GetVertices());

		UpdateBox();
	}

	void BoxComponent::Tick(const float& DeltaTime)
	{
		Super::Tick(DeltaTime);

		UpdateBox();
	}

	void BoxComponent::Render(IRenderer* r)
	{
		if (!IsVisible())
			return;
		Super::Render(r);

		DrawMode temp = r->GetDrawMode();
		r->SetDrawMode(DrawMode::Wireframe);
		IRenderer3D* r3d = dynamic_cast<IRenderer3D*>(r);
		if (r3d == nullptr)
			return;

		r3d->DrawMesh(_mesh, _transform.get(), nullptr);
		r->SetDrawMode(temp);
	}

	bool BoxComponent::CheckCollision(Collider* other)
	{
		if (Super::CheckCollision(other) == false)
			return false;
		
		Matrix4x4 mat = _transform->GetTransformMatrix();
		Box box = { (Vector4(_bound.Min) * mat).ToVector3(), (Vector4(_bound.Max) * mat).ToVector3() };

		switch (other->GetColliderType())
		{
		case ColliderType::Box: {
			BoxComponent* box2 = dynamic_cast<BoxComponent*>(other);
			Matrix4x4 otherMat = box2->GetTransformMatrix();
			Box otherBox = { (Vector4(box2->GetBox().Min) * otherMat).ToVector3(), (Vector4(box2->GetBox().Max) * otherMat).ToVector3() };
			return box.Intersect(otherBox);
		}
		case ColliderType::Sphere:
			return CheckCollisionSphereToBox(dynamic_cast<SphereComponent*>(other)->GetSphere(), box);
		}

		return false;
	}

	void BoxComponent::UpdateBox()
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
	}

	const Matrix4x4& BoxComponent::GetTransformMatrix()
	{
		return _transform->GetTransformMatrix();
	}
};