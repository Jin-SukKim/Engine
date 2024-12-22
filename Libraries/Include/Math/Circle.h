#pragma once

namespace JE {

struct Vertex2D;

struct Circle
{
	Circle() = default;
	Circle(const Circle& circle) : Center(circle.Center), Radius(circle.Radius) {}
	// Mesh의 Bounding Volume으로 사용하기 위한 생성자
	Circle(const std::vector<Vertex2D>& vertices);

	// Point가 원 안에 있는지 확인
	bool IsInside(const Vector2& v) const;
	// 다른 원과 겹치는지 확인
	bool Intersect(const Circle& c) const;

	Vector2 Center = Vector2::Zero;
	float Radius = 0.f;
};

}
