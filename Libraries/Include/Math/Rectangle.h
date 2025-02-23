#pragma once

namespace JE {

struct Vertex2D;

struct Rectangle
{
	Rectangle() = default;
	Rectangle(const Rectangle& rect) : Min(rect.Min), Max(rect.Max) {}
	Rectangle(const Vector2& min, const Vector2& max) : Min(min), Max(max) {}
	// Mesh 데이터를 받아 Mesh의 사각형 Bounding Volume 설정
	Rectangle(const std::vector<Vertex2D>& vertices);
	
	// 사각형 영역이 겹치는지 판별
	bool Intersect(const Rectangle& rect) const;
	// 다른 사각형이 안에 있는지 판별
	bool IsInside(const Rectangle& rect) const;
	// Point가 안에 있는지 판별
	bool IsInside(const Vector2& pos) const;
	
	// return Vector2(width, height)
	Vector2 GetSize() const;

	void UpdateMinMax(const Vector2& v);
	void UpdateMinMax(const Rectangle& rect);

	Vector2 Min;
	Vector2 Max;
};

}