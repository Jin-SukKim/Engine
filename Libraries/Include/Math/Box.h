#pragma once

namespace JE{

struct Vertex3D;

struct Box
{
	Box() = default;
	Box(const Box& box) : Min(box.Min), Max(box.Max) {}
	Box(const Vector3& min, const Vector3& max) : Min(min), Max(max) {}
	// Mesh의 Bounding Area (AABB)
	Box(const std::vector<Vertex3D>& vertices);

	// 겹치는지 판별
	bool Intersect(const Box& box) const;
	// 다른 Box가 Box 안에 있는지 판별
	bool IsInside(const Box& box) const;
	// Point가 Box안에 있는지 판별
	bool IsInside(const Vector3& pos) const;

	void UpdateMinMax(const Vector3& pos);
	void UpdateMinMax(const Box& box);

	// 박스 크기 (return (width, height, depth))
	Vector3 GetSize() const;

	Vector3 Min; // 박스 영역의 최솟값
	Vector3 Max; // 박스 영역의 최댓값
};

}
