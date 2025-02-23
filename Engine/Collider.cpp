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
		// Collision Manager에서 nullptr을 확인했음
		if (!_enable || !other->GetCollisionEnable())
			return false;

		return true;
	}

	bool Collider::CheckCollisionCircleToRect(const Circle& c1, const Rectangle& b1)
	{
		// 원의 반지름만큼 사각형을 확장한 뒤, 원의 중심점이 확장한 사각형 안에 있다면 충돌
		Rectangle exRect = {
				b1.Min - c1.Radius,
				b1.Max + c1.Radius
		};
		if (exRect.IsInside(c1.Center))
			return true;

		// 예외적으로 원래 사각형의 대각선 쪽으로 왔을땐 사각형 꼭지점들이 원 안에 있는지 확인 (대각선은 상하좌우보다 더 멀리 있을 수 있음)
		
		// 사각형 좌상단 꼭지점이 원 안에 있는지 확인
		Vector2 point = { b1.Min.X, b1.Max.Y };
		if (c1.IsInside(point))
			return true;
		// 사각형 좌하단 꼭지점이 원 안에 있는지 확인
		if (c1.IsInside(b1.Min))
			return true;
		// 사각형 우하단 꼭지점이 원 안에 있는지 확인
		point.X = b1.Max.X;
		point.Y = b1.Min.Y;
		if (c1.IsInside(point))
			return true;
		// 사각형 우상단 꼭지점이 원 안에 있는지 확인
		if (c1.IsInside(b1.Max))
			return true;

		return false;
	}
	bool Collider::CheckCollisionSphereToBox(const Sphere& s1, const Box& b1)
	{
		// 원 정보
		const Vector3 circlePos = s1.Center;
		const float radius = s1.Radius;

		// 원의 반지름만큼 사각형을 확장한 뒤, 원의 중심점이 확장한 사각형 안에 있다면 충돌
		Box exRect = {
				b1.Min - s1.Radius,
				b1.Max + s1.Radius
		};
		if (exRect.IsInside(circlePos))
			return true;

		// 예외적으로 원래 사각형의 대각선 쪽으로 왔을땐 사각형 꼭지점들이 원 안에 있는지 확인 (대각선은 상하좌우보다 더 멀리 있을 수 있음)
		// 8개의 꼭지점을 순회
		for (int i = 0; i < 8; ++i) // 숫자 8은 3 bit
		{
			Vector3 corner;
			// 3비트를 사용하여 각 축의 Min/Max 선택 (bit 연산 사용)
			// i & 1 : X축 (0: Min.X, 1: Max.X)
			// i & 2 : Y축 (0: Min.Y, 1: Max.Y)
			// i & 4 : Z축 (0: Min.Z, 1: Max.Z)
			corner.X = (i & 1) ? b1.Max.X : b1.Min.X;
			corner.Y = (i & 2) ? b1.Max.Y : b1.Min.Y;
			corner.Z = (i & 4) ? b1.Max.Z : b1.Min.Z;

			if (s1.IsInside(corner))
				return true;
		}


		return false;
	}
}