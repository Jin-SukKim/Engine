#include "pch.h"
#include "Box.h"

namespace JE {
	Box::Box(const std::vector<Vertex3D>& vertices)
	{
		for (const Vertex3D& v : vertices)
			UpdateMinMax(v.Pos.ToVector3());
	}
	bool Box::Intersect(const Box& box) const
	{
		// x 범위를 벗어나는지 판별
		if ((Min.X > box.Max.X) || (box.Min.X > Max.X))
			return false;

		// y 범위를 벗어나는지 판별
		if ((Min.Y > box.Max.Y) || (box.Min.Y > Max.Y))
			return false;

		// z 범위를 벗어나느지 판별
		if ((Min.Z > box.Max.Z) || (box.Min.Z > Max.Z))
			return false;

		return true;
	}
	bool Box::IsInside(const Box& box) const
	{
		return IsInside(box.Min) && IsInside(box.Max);
	}
	bool Box::IsInside(const Vector3& pos) const
	{
		return
			// x 범위 안엥 있는지 판별
			((pos.X >= Min.X) && (pos.X <= Max.X) &&
			// y 범위 안에 있는지 판별
			(pos.Y >= Min.Y) && (pos.Y <= Max.Y) &&
			// z 범위 안에 있는지 판별
			(pos.Z >= Min.Z) && (pos.Z <= Max.Z));
	}
	void Box::UpdateMinMax(const Vector3& pos)
	{
		Min.X = std::min(Min.X, pos.X);
		Min.Y = std::min(Min.Y, pos.Y);
		Min.Z = std::min(Min.Z, pos.Z);

		Max.X = std::max(Max.X, pos.X);
		Max.Y = std::max(Max.Y, pos.Y);
		Max.Z = std::max(Max.Z, pos.Z);
	}
	void Box::UpdateMinMax(const Box& box)
	{
		Min.X = std::min(Min.X, box.Min.X);
		Min.Y = std::min(Min.Y, box.Min.Y);
		Min.Z = std::min(Min.Z, box.Min.Z);

		Max.X = std::max(Max.X, box.Max.X);
		Max.Y = std::max(Max.Y, box.Max.Y);
		Max.Z = std::max(Max.Z, box.Max.Z);
	}
	Vector3 Box::GetSize() const
	{
		return Max - Min;
	}
}