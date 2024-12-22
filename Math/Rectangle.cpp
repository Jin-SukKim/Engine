#include "pch.h"
#include "Rectangle.h"


namespace JE {
	Rectangle::Rectangle(const std::vector<Vertex2D>& vertices)
	{
		// Mesh의 Vertex 중 최소값과 최대값을 찾아서 Mesh의 크기에 맞게 갱신
		for (const Vertex2D& v : vertices)
			UpdateMinMax(v.Pos);
	}
	bool Rectangle::Intersect(const Rectangle& rect) const
	{
		// 다른 사각형이 사각형의 왼쪽이나 오른쪽에 있다면 false
		if ((Min.X > rect.Max.X) || (rect.Min.X > Max.X))
			return false;

		// 다른 사각형이 사각형의 위나 아래에 있다면 false
		if ((Min.Y > rect.Max.Y) || (rect.Min.Y > Max.Y))
			return false;

		return true;
	}
	bool Rectangle::IsInside(const Rectangle& rect) const
	{
		// 사각형의 최솟값과 최대값이 사각형 안에 있다면
		return IsInside(rect.Min) && IsInside(rect.Max);
	}
	bool Rectangle::IsInside(const Vector2& pos) const
	{
		return (
			// Point의 위치가 사각형의 x 범위 안에 있는지 확인
			(pos.X >= Min.X) && (pos.X <= Max.X) &&
			// Point의 위치가 사각형의 y 범위 안에 있는지 확인
			(pos.Y >= Min.Y) && (pos.Y <= Max.Y)
		);
	}
	Vector2 Rectangle::GetSize() const
	{
		return Max - Min;
	}
	void Rectangle::UpdateMinMax(const Vector2& v)
	{
		Min.X = std::min(Min.X, v.X);
		Min.Y = std::min(Min.Y, v.Y);

		Max.X = std::max(Max.X, v.X);
		Max.Y = std::max(Max.Y, v.Y);
	}
	void Rectangle::UpdateMinMax(const Rectangle& rect)
	{
		Min.X = std::min(Min.X, rect.Min.X);
		Min.Y = std::min(Min.Y, rect.Min.Y);

		Max.X = std::max(Max.X, rect.Max.X);
		Max.Y = std::max(Max.Y, rect.Max.Y);
	}
}