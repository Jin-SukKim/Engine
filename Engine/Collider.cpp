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
		// Collision Manager에서 nullptr을 확인했음
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

		RECT intersect = {}; // 교차점 (겹치는 위치)

		bool check = ::IntersectRect(&intersect, &r1, &r2);

		return check;
	}
	bool Collider::CheckCollisionCircleToRect(CircleComponent* c1, RectangleComponent* b1)
	{
		if (c1 == nullptr || b1 == nullptr)
			return false;

		// 사각형 정보
		const RECT square = b1->GetRect();
		
		// 원 정보
		const Vector2 circlePos = c1->GetPos();
		const float radius = c1->GetRadius();

		bool horizontal = square.left <= circlePos.X && circlePos.X <= square.right;
		bool vertical = square.top <= circlePos.Y && circlePos.Y <= square.bottom;

		// 원의 반지름만큼 사각형을 확장한 뒤, 원의 중심점이 확장한 사각형 안에 있다면 충돌
		if (horizontal || vertical) {
			// 원의 반지름만큼 사각형을 확장
			RECT exRect = {
				square.left - static_cast<LONG>(radius),
				square.top - static_cast<LONG>(radius),
				square.right + static_cast<LONG>(radius),
				square.bottom + static_cast<LONG>(radius)
			};

			// 확장한 사각형 안에 원의 중심이 있는지 확인
			if (exRect.left < circlePos.X && circlePos.X < exRect.right &&
				exRect.top < circlePos.Y && circlePos.Y < exRect.bottom)
				return true;
		}
		// 예외적으로 원래 사각형의 대각선 쪽으로 왔을땐 사각형 꼭지점들이 원 안에 있는지 확인 (대각선은 상하좌우보다 더 멀리 있을 수 있음)
		else {
			// 사각형 좌상단 꼭지점이 원 안에 있는지 확인
			Vector2 point = { static_cast<float>(square.left), static_cast<float>(square.top) };
			if (CheckPointInCircle(circlePos, radius, point))
				return true;
			// 사각형 좌하단 꼭지점이 원 안에 있는지 확인
			point.Y = static_cast<float>(square.bottom);
			if (CheckPointInCircle(circlePos, radius, point))
				return true;
			// 사각형 우하단 꼭지점이 원 안에 있는지 확인
			point.X = static_cast<float>(square.right);
			if (CheckPointInCircle(circlePos, radius, point))
				return true;
			// 사각형 우상단 꼭지점이 원 안에 있는지 확인
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

		// 두 원의 중점들끼리의 거리
		const float circleDist = c1Radius + c2Radius;

		// 두 원의 각각의 중점끼리의 거리가 두 원의 반지름의 합보다 작다면 충돌
		return dist <= circleDist * circleDist;
	}
	bool Collider::CheckPointInCircle(const Vector2& cPos, const float& radius, const Vector2& point)
	{
		// 원과 점 사이의 거리
		Vector2 v = cPos - point;
		float distSq = v.SizeSquared();

		// 원의 반지름보다 점과 원의 거리가 작다면 점은 원 안에 존재
		if (distSq > radius * radius)
			return false;

		return true;
	}
}