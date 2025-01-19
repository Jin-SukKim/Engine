#include "pch.h"
#include "Collider.h"

namespace JE {
	uint32 Collider::_collisionID = 0;

	Collider::Collider(ColliderType type, const std::wstring& name) : Component(name, ComponentType::Collider), _id(_collisionID++), _type(type)
	{
	}


	void Collider::Render(IRenderer* r)
	{
		Super::Render(r);
	}

	void Collider::OnCollisionBeginOverlap(Collider* other)
	{
		BeginOverlap.Execute(this, other);
	}

	void Collider::OnCollisionOverlap(Collider* other)
	{
		Overlap.Execute(this, other);
	}

	void Collider::OnCollisionEndOverlap(Collider* other)
	{
		EndOverlap.Execute(this, other);
	}
	bool Collider::CheckCollision(Collider* other)
	{
		// Collision Manager에서 nullptr을 확인했음
		if (!_enable || !other->GetCollisionEnable())
			return false;

		return true;
	}
}