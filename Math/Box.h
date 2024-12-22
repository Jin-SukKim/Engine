#pragma once

namespace JE{

struct Vertex3D;

struct Box
{
	Box() = default;
	Box(const Box& box) : Min(box.Min), Max(box.Max) {}
	Box(const Vector3& min, const Vector3& max) : Min(min), Max(max) {}
	// Mesh�� Bounding Area (AABB)
	Box(const std::vector<Vertex3D>& vertices);

	// ��ġ���� �Ǻ�
	bool Intersect(const Box& box) const;
	// �ٸ� Box�� Box �ȿ� �ִ��� �Ǻ�
	bool IsInside(const Box& box) const;
	// Point�� Box�ȿ� �ִ��� �Ǻ�
	bool IsInside(const Vector3& pos) const;

	void UpdateMinMax(const Vector3& pos);
	void UpdateMinMax(const Box& box);

	// �ڽ� ũ�� (return (width, height, depth))
	Vector3 GetSize() const;

	Vector3 Min; // �ڽ� ������ �ּڰ�
	Vector3 Max; // �ڽ� ������ �ִ�
};

}
