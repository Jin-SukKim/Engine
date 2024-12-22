#pragma once

namespace JE {

struct Vertex3D;

struct Sphere
{
	Sphere() = default;
	Sphere(const Circle& circle) : Center(circle.Center), Radius(circle.Radius) {}
	// Bounding Volume
	Sphere(const std::vector<Vertex3D>& vertices);

	// �� ���� �ȿ� Point�� �����ϴ��� ����
	bool IsInside(const Vector3& v) const;
	// �� ���� ������ ���� ��ġ���� ����
	bool Intersect(const Sphere& sphere) const;
	
	Vector3 Center = Vector3::Zero;
	float Radius = 0.f;
};

}
