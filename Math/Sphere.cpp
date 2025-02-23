#include "pch.h"
#include "Sphere.h"

namespace JE {
	Sphere::Sphere(const std::vector<Vertex3D>& vertices)
	{
		// Mesh�� ������ ��
		UpdateRadius(vertices);
	}

	bool Sphere::IsInside(const Vector3& v) const
	{
		return (Center - v).SizeSquared() <= (Radius * Radius);
	}

	bool Sphere::Intersect(const Sphere& sphere) const
	{
		// �� ���� �������� ��
		float radiusSum = Radius + sphere.Radius;
		// �� ���� ������ �Ÿ��� �������� �պ��� �۴ٸ� �����ִ� ��
		return (Center - sphere.Center).SizeSquared() < (radiusSum * radiusSum);
	}
	void Sphere::UpdateRadius(const std::vector<Vertex3D>& vertices)
	{
		size_t count = vertices.size();
		if (count == 0)
			return;

		// ��� ���� ��ǥ�� �����ֱ�
		Vector3 sum;
		for (const Vertex3D& v : vertices)
			sum += v.Pos.ToVector3();

		// ��ճ��� Mesh�� ���� ã��
		Center = sum / static_cast<float>(count);

		// �������� ���� �� ���� ã��
		Vector3 farthestPoint = (*max_element(vertices.begin(), vertices.end(),
			[&](const Vertex3D& l, const Vertex3D& r) {
				return (Center - l.Pos.ToVector3()).SizeSquared() < (Center - r.Pos.ToVector3()).SizeSquared();
			})).Pos.ToVector3();

		Radius = (farthestPoint - Center).Size();
	}
}