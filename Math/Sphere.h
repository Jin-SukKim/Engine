#pragma once

namespace JE {

struct Vertex3D;

struct Sphere
{
	Sphere() = default;
	Sphere(const Circle& circle) : Center(circle.Center), Radius(circle.Radius) {}
	// Bounding Volume
	Sphere(const std::vector<Vertex3D>& vertices);

	// 구 영역 안에 Point을 포함하는지 판정
	bool IsInside(const Vector3& v) const;
	// 두 구의 영역이 서로 겹치는지 판정
	bool Intersect(const Sphere& sphere) const;
	
	Vector3 Center = Vector3::Zero;
	float Radius = 0.f;
};

}
