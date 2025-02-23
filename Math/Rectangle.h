#pragma once

namespace JE {

struct Vertex2D;

struct Rectangle
{
	Rectangle() = default;
	Rectangle(const Rectangle& rect) : Min(rect.Min), Max(rect.Max) {}
	Rectangle(const Vector2& min, const Vector2& max) : Min(min), Max(max) {}
	// Mesh �����͸� �޾� Mesh�� �簢�� Bounding Volume ����
	Rectangle(const std::vector<Vertex2D>& vertices);
	
	// �簢�� ������ ��ġ���� �Ǻ�
	bool Intersect(const Rectangle& rect) const;
	// �ٸ� �簢���� �ȿ� �ִ��� �Ǻ�
	bool IsInside(const Rectangle& rect) const;
	// Point�� �ȿ� �ִ��� �Ǻ�
	bool IsInside(const Vector2& pos) const;
	
	// return Vector2(width, height)
	Vector2 GetSize() const;

	void UpdateMinMax(const Vector2& v);
	void UpdateMinMax(const Rectangle& rect);

	Vector2 Min;
	Vector2 Max;
};

}