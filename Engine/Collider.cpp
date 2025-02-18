#include "pch.h"
#include "Collider.h"
#include "RectangleComponent.h"
#include "CircleComponent.h"

namespace JE {
	uint32 Collider::_collisionID = 0;

	Collider::Collider(ColliderType type, const std::wstring& name) : Component(name, ComponentType::Collider), _id(_collisionID++), _type(type)
	{
	}


	void Collider::Render(IRenderer* r)
	{
		if (!_show)
			return;
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
		// Collision Manager���� nullptr�� Ȯ������
		if (!_enable || !other->GetCollisionEnable())
			return false;

		return true;
	}

	bool Collider::CheckCollisionRectToRect(RectangleComponent* b1, RectangleComponent* b2)
	{
		if (b1 == nullptr || b2 == nullptr)
			return false;

		RECT r1 = b1->GetRect();
		RECT r2 = b2->GetRect();

		RECT intersect = {}; // ������ (��ġ�� ��ġ)

		bool check = ::IntersectRect(&intersect, &r1, &r2);

		return check;
	}
	bool Collider::CheckCollisionCircleToRect(CircleComponent* c1, RectangleComponent* b1)
	{
		if (c1 == nullptr || b1 == nullptr)
			return false;

		// �簢�� ����
		const RECT square = b1->GetRect();
		
		// �� ����
		const Vector2 circlePos = c1->GetPos();
		const float radius = c1->GetRadius();

		bool horizontal = square.left <= circlePos.X && circlePos.X <= square.right;
		bool vertical = square.top <= circlePos.Y && circlePos.Y <= square.bottom;

		// ���� ��������ŭ �簢���� Ȯ���� ��, ���� �߽����� Ȯ���� �簢�� �ȿ� �ִٸ� �浹
		if (horizontal || vertical) {
			// ���� ��������ŭ �簢���� Ȯ��
			RECT exRect = {
				square.left - static_cast<LONG>(radius),
				square.top - static_cast<LONG>(radius),
				square.right + static_cast<LONG>(radius),
				square.bottom + static_cast<LONG>(radius)
			};

			// Ȯ���� �簢�� �ȿ� ���� �߽��� �ִ��� Ȯ��
			if (exRect.left < circlePos.X && circlePos.X < exRect.right &&
				exRect.top < circlePos.Y && circlePos.Y < exRect.bottom)
				return true;
		}
		// ���������� ���� �簢���� �밢�� ������ ������ �簢�� ���������� �� �ȿ� �ִ��� Ȯ�� (�밢���� �����¿캸�� �� �ָ� ���� �� ����)
		else {
			// �簢�� �»�� �������� �� �ȿ� �ִ��� Ȯ��
			Vector2 point = { static_cast<float>(square.left), static_cast<float>(square.top) };
			if (CheckPointInCircle(circlePos, radius, point))
				return true;
			// �簢�� ���ϴ� �������� �� �ȿ� �ִ��� Ȯ��
			point.Y = static_cast<float>(square.bottom);
			if (CheckPointInCircle(circlePos, radius, point))
				return true;
			// �簢�� ���ϴ� �������� �� �ȿ� �ִ��� Ȯ��
			point.X = static_cast<float>(square.right);
			if (CheckPointInCircle(circlePos, radius, point))
				return true;
			// �簢�� ���� �������� �� �ȿ� �ִ��� Ȯ��
			point.Y = static_cast<float>(square.top);
			if (CheckPointInCircle(circlePos, radius, point))
				return true;
		}

		return false;
	}
	bool Collider::CheckCollisionCircleToCircle(CircleComponent* c1, CircleComponent* c2)
	{
		if (c1 == nullptr || c2 == nullptr)
			return false;

		const Vector2 c1Pos = c1->GetPos();
		const float c1Radius = c1->GetRadius();

		const Vector2 c2Pos = c2->GetPos();
		const float c2Radius = c2->GetRadius();
		
		Vector2 dir = c2Pos - c1Pos;
		const float dist = dir.SizeSquared();

		// �� ���� �����鳢���� �Ÿ�
		const float circleDist = c1Radius + c2Radius;

		// �� ���� ������ ���������� �Ÿ��� �� ���� �������� �պ��� �۴ٸ� �浹
		return dist <= circleDist * circleDist;
	}
	bool Collider::CheckPointInCircle(const Vector2& cPos, const float& radius, const Vector2& point)
	{
		// ���� �� ������ �Ÿ�
		Vector2 v = cPos - point;
		float distSq = v.SizeSquared();

		// ���� ���������� ���� ���� �Ÿ��� �۴ٸ� ���� �� �ȿ� ����
		if (distSq > radius * radius)
			return false;

		return true;
	}
}