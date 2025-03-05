#include "pch.h"
#include "RectangleComponent.h"
#include "CircleComponent.h"
#include "AssetManager.h"
#include "SpriteActor.h"
#include "Transform2DComponent.h"
#include "Renderer/CppRenderer2D.h"
#include "Mesh2D.h"
#include "Math/Rectangle.h"
#include "Math/Circle.h"

namespace JE {
	void RectangleComponent::Init()
	{
		Super::Init();

		AssetManager::Load<Mesh2D>(L"SquareMesh", MeshData::SquareVertex, MeshData::SquareIndices);

		_mesh = AssetManager::Find<Mesh2D>(L"SquareMesh");
		_transform = std::make_unique<Transform2DComponent>(L"BoundingTransform");
		_bound = Rectangle(_mesh->GetVertices());

		UpdateRect();
	}

	void RectangleComponent::Tick(const float& DeltaTime)
	{
		Super::Tick(DeltaTime);
		
		UpdateRect();
	}

	void RectangleComponent::Render(IRenderer* r)
	{
		if (!IsVisible())
			return;
		Super::Render(r);

		DrawMode temp = r->GetDrawMode();
		r->SetDrawMode(DrawMode::Wireframe);
		IRenderer2D* r2d = dynamic_cast<IRenderer2D*>(r);
		if (r2d == nullptr)
			return;
		r2d->DrawMesh(_mesh, _transform.get(), nullptr);
		r->SetDrawMode(temp);
	}

	bool RectangleComponent::CheckCollision(Collider* other)
	{
		if (Super::CheckCollision(other) == false)
			return false;

		Matrix3x3 mat = _transform->GetTransformMatrix();
		Rectangle rect = { _bound.Min * mat, _bound.Max * mat };

		switch (other->GetColliderType())
		{ 
		case ColliderType::Rectangle: {
			RectangleComponent* rect2 = dynamic_cast<RectangleComponent*>(other);
			Matrix3x3 otherMat = rect2->GetTransformMatrix();
			Rectangle otherRect = { rect2->GetRect().Min * otherMat, rect2->GetRect().Max * otherMat };
			return rect.Intersect(otherRect);
		}
		case ColliderType::Circle:	
			return CheckCollisionCircleToRect(dynamic_cast<CircleComponent*>(other)->GetCircle(), rect);
		}

		return false;
	}

	void RectangleComponent::UpdateRect()
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
	}

	const Matrix3x3& RectangleComponent::GetTransformMatrix()
	{
		return _transform->GetTransformMatrix();
	}
}