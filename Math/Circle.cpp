#include "pch.h"
#include "Circle.h"

namespace JE {
	Circle::Circle(const std::vector<Vertex2D>& vertices)
	{
		// Mesh �����κ��� �� ������ ����
		UpdateRadius(vertices);
	}

	bool Circle::IsInside(const Vector2& v) const
	{
		// ���� ������ �� ������ �Ÿ��� ���������� �۰ų� ������ �� �ȿ� �ִ� ��
		return (Center - v).SizeSquared() <= (Radius * Radius); // �������� sqrt ��� ����� �� ŭ
	}

	bool Circle::Intersect(const Circle& c) const
	{
		// �� ���� �������� ��
		float twoRadiusSum = Radius + c.Radius;
		// �� ���� ������ �Ÿ��� �� ���� �������� ���� ������ �۴ٸ� �����ִ� 
		return (Center - c.Center).SizeSquared() < (twoRadiusSum * twoRadiusSum);
	}
	void Circle::UpdateRadius(const std::vector<Vertex2D>& vertices)
	{
		// �޽��� ������ ��
		size_t count = vertices.size();
		if (count == 0)
			return;

		// ��� ������ ��ġ�� �����ֱ�
		Vector2 sum;
		for (const Vertex2D& v : vertices)
			sum += v.Pos;

		// ������ ���� ���� ��ġ ���� ��� �� ����(���� �߽�)�� ��ġ�� ���
		Center = sum / static_cast<float>(count);
		// �������κ��� ��� ���� �Ÿ��� ���ϰ� �� �߿��� ���� ū ���� ���������� ����
		Vector2 farthestPoint = (*std::max_element(vertices.begin(), vertices.end(),
			[&](const Vertex2D& l, const Vertex2D& r) {
				// (Center - point).SizeSquared() = �������� point������ �Ÿ� (sqrt�� ���� �ʰ� ��귮 ���̱�)
				return (Center - l.Pos).SizeSquared() < (Center - r.Pos).SizeSquared();
			})).Pos;

		Radius = (farthestPoint - Center).Size();
	}
}