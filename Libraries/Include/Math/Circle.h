#pragma once

namespace JE {

struct Vertex2D;

struct Circle
{
	Circle() = default;
	Circle(const Circle& circle) : Center(circle.Center), Radius(circle.Radius) {}
	// Mesh�� Bounding Volume���� ����ϱ� ���� ������
	Circle(const std::vector<Vertex2D>& vertices);

	// Point�� �� �ȿ� �ִ��� Ȯ��
	bool IsInside(const Vector2& v) const;
	// �ٸ� ���� ��ġ���� Ȯ��
	bool Intersect(const Circle& c) const;

	Vector2 Center = Vector2::Zero;
	float Radius = 0.f;
};

}
