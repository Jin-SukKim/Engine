#include "pch.h"
#include "Rectangle.h"


namespace JE {
	Rectangle::Rectangle(const std::vector<Vertex2D>& vertices)
	{
		// Mesh�� Vertex �� �ּҰ��� �ִ밪�� ã�Ƽ� Mesh�� ũ�⿡ �°� ����
		for (const Vertex2D& v : vertices)
			UpdateMinMax(v.Pos);
	}
	bool Rectangle::Intersect(const Rectangle& rect) const
	{
		// �ٸ� �簢���� �簢���� �����̳� �����ʿ� �ִٸ� false
		if ((Min.X > rect.Max.X) || (rect.Min.X > Max.X))
			return false;

		// �ٸ� �簢���� �簢���� ���� �Ʒ��� �ִٸ� false
		if ((Min.Y > rect.Max.Y) || (rect.Min.Y > Max.Y))
			return false;

		return true;
	}
	bool Rectangle::IsInside(const Rectangle& rect) const
	{
		// �簢���� �ּڰ��� �ִ밪�� �簢�� �ȿ� �ִٸ�
		return IsInside(rect.Min) && IsInside(rect.Max);
	}
	bool Rectangle::IsInside(const Vector2& pos) const
	{
		return (
			// Point�� ��ġ�� �簢���� x ���� �ȿ� �ִ��� Ȯ��
			(pos.X >= Min.X) && (pos.X <= Max.X) &&
			// Point�� ��ġ�� �簢���� y ���� �ȿ� �ִ��� Ȯ��
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