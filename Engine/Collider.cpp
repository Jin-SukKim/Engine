#include "pch.h"
#include "Collider.h"
#include "Math/Rectangle.h"
#include "Math/Circle.h"
#include "Math/Box.h"
#include "Math/Sphere.h"


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

	bool Collider::CheckCollisionCircleToRect(const Circle& c1, const Rectangle& b1)
	{
		// ���� ��������ŭ �簢���� Ȯ���� ��, ���� �߽����� Ȯ���� �簢�� �ȿ� �ִٸ� �浹
		Rectangle exRect = {
				b1.Min - c1.Radius,
				b1.Max + c1.Radius
		};
		if (exRect.IsInside(c1.Center))
			return true;

		// ���������� ���� �簢���� �밢�� ������ ������ �簢�� ���������� �� �ȿ� �ִ��� Ȯ�� (�밢���� �����¿캸�� �� �ָ� ���� �� ����)
		
		// �簢�� �»�� �������� �� �ȿ� �ִ��� Ȯ��
		Vector2 point = { b1.Min.X, b1.Max.Y };
		if (c1.IsInside(point))
			return true;
		// �簢�� ���ϴ� �������� �� �ȿ� �ִ��� Ȯ��
		if (c1.IsInside(b1.Min))
			return true;
		// �簢�� ���ϴ� �������� �� �ȿ� �ִ��� Ȯ��
		point.X = b1.Max.X;
		point.Y = b1.Min.Y;
		if (c1.IsInside(point))
			return true;
		// �簢�� ���� �������� �� �ȿ� �ִ��� Ȯ��
		if (c1.IsInside(b1.Max))
			return true;

		return false;
	}
	bool Collider::CheckCollisionSphereToBox(const Sphere& s1, const Box& b1)
	{
		// �� ����
		const Vector3 circlePos = s1.Center;
		const float radius = s1.Radius;

		// ���� ��������ŭ �簢���� Ȯ���� ��, ���� �߽����� Ȯ���� �簢�� �ȿ� �ִٸ� �浹
		Box exRect = {
				b1.Min - s1.Radius,
				b1.Max + s1.Radius
		};
		if (exRect.IsInside(circlePos))
			return true;

		// ���������� ���� �簢���� �밢�� ������ ������ �簢�� ���������� �� �ȿ� �ִ��� Ȯ�� (�밢���� �����¿캸�� �� �ָ� ���� �� ����)
		// 8���� �������� ��ȸ
		for (int i = 0; i < 8; ++i) // ���� 8�� 3 bit
		{
			Vector3 corner;
			// 3��Ʈ�� ����Ͽ� �� ���� Min/Max ���� (bit ���� ���)
			// i & 1 : X�� (0: Min.X, 1: Max.X)
			// i & 2 : Y�� (0: Min.Y, 1: Max.Y)
			// i & 4 : Z�� (0: Min.Z, 1: Max.Z)
			corner.X = (i & 1) ? b1.Max.X : b1.Min.X;
			corner.Y = (i & 2) ? b1.Max.Y : b1.Min.Y;
			corner.Z = (i & 4) ? b1.Max.Z : b1.Min.Z;

			if (s1.IsInside(corner))
				return true;
		}


		return false;
	}
}