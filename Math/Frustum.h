#pragma once

namespace JE {

enum class BoundCheckResult {
	Outside = 0,
	Intersect,
	Inside
};

struct Frustum
{
	Frustum() = default;
	Frustum(const std::array<Plane, 6>& planes) : Planes(planes) {}

	// �� ������ ���� ����ü�� ���� �ִ����� �Ǻ� (���� : �ܺ�, ��ĥ��, ����)
	BoundCheckResult CheckBound(const Vector3& point) const;
	// �� �ٿ�� ������ ����� ����ü �ø�
	BoundCheckResult CheckBound(const Sphere& sphere) const;
	// �ڽ� �ٿ�� ����(AABB)�� ����� ����ü �ø�
	BoundCheckResult CheckBound(const Box& box) const;

	// ����� �������� Ȱ���� ����ü
	// Y-(����), Y+(����), X-(�ϴ�), X+(���), Z-(�����), Z+(�����) ������ ����
	std::array<Plane, 6> Planes;
};

}
