#include "pch.h"
#include "Circle.h"

namespace JE {
	Circle::Circle(const std::vector<Vertex2D>& vertices)
	{
		// Mesh 정보로부터 원 영역을 생성
		UpdateRadius(vertices);
	}

	bool Circle::IsInside(const Vector2& v) const
	{
		// 원의 중점과 점 사이의 거리가 반지름보다 작거나 같으면 원 안에 있는 것
		return (Center - v).SizeSquared() <= (Radius * Radius); // 곱셈보다 sqrt 계산 비용이 더 큼
	}

	bool Circle::Intersect(const Circle& c) const
	{
		// 두 원의 반지름의 합
		float twoRadiusSum = Radius + c.Radius;
		// 두 원의 중점의 거리가 두 원의 반지름을 더한 값보다 작다면 겹쳐있는 
		return (Center - c.Center).SizeSquared() < (twoRadiusSum * twoRadiusSum);
	}
	void Circle::UpdateRadius(const std::vector<Vertex2D>& vertices)
	{
		// 메시의 정점의 수
		size_t count = vertices.size();
		if (count == 0)
			return;

		// 모든 정점의 위치를 더해주기
		Vector2 sum;
		for (const Vertex2D& v : vertices)
			sum += v.Pos;

		// 정점의 수로 나눠 위치 값을 평균 내 중점(원의 중심)의 위치를 계산
		Center = sum / static_cast<float>(count);
		// 중점으로부터 모든 점의 거리를 구하고 이 중에서 가장 큰 값을 반지름으로 지정
		Vector2 farthestPoint = (*std::max_element(vertices.begin(), vertices.end(),
			[&](const Vertex2D& l, const Vertex2D& r) {
				// (Center - point).SizeSquared() = 중점에서 point까지의 거리 (sqrt를 하지 않고 계산량 줄이기)
				return (Center - l.Pos).SizeSquared() < (Center - r.Pos).SizeSquared();
			})).Pos;

		Radius = (farthestPoint - Center).Size();
	}
}