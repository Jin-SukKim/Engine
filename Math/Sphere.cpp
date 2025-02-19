#include "pch.h"
#include "Sphere.h"

namespace JE {
	Sphere::Sphere(const std::vector<Vertex3D>& vertices)
	{
		// Mesh의 정점의 수
		UpdateRadius(vertices);
	}

	bool Sphere::IsInside(const Vector3& v) const
	{
		return (Center - v).SizeSquared() <= (Radius * Radius);
	}

	bool Sphere::Intersect(const Sphere& sphere) const
	{
		// 두 구의 반지름의 합
		float radiusSum = Radius + sphere.Radius;
		// 두 구의 중점의 거리가 반지름의 합보다 작다면 겹쳐있는 것
		return (Center - sphere.Center).SizeSquared() < (radiusSum * radiusSum);
	}
	void Sphere::UpdateRadius(const std::vector<Vertex3D>& vertices)
	{
		size_t count = vertices.size();
		if (count == 0)
			return;

		// 모든 점의 조표를 더해주기
		Vector3 sum;
		for (const Vertex3D& v : vertices)
			sum += v.Pos.ToVector3();

		// 평균내서 Mesh의 중점 찾기
		Center = sum / static_cast<float>(count);

		// 중점에서 가장 먼 정점 찾기
		Vector3 farthestPoint = (*max_element(vertices.begin(), vertices.end(),
			[&](const Vertex3D& l, const Vertex3D& r) {
				return (Center - l.Pos.ToVector3()).SizeSquared() < (Center - r.Pos.ToVector3()).SizeSquared();
			})).Pos.ToVector3();

		Radius = (farthestPoint - Center).Size();
	}
}