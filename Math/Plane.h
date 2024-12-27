#pragma once

namespace JE {

struct Plane
{
	Plane() = default;
	// ����� �� �� �Ǵ� ���� ���Ϳ� ��� ���� �� ������ ����
	Plane(const Vector3& normal, const float& d);
	Plane(const Vector3& normal, const Vector3 planePt);
	// ����� �����ϴ� ����
	Plane(const Vector3& pos1, const Vector3& pos2, const Vector3& pos3);
	// ����� �����ϴ� a, b, c, d�� Vector4�� �־��� �� (����� ������ : ax + by + cz + d = 0)
	Plane(const Vector4& v);

	// point < 0 : ����� ����, point == 0 : ����� ��, 0 < point : ����� �ٱ�, ���밪�� ��鿡�� ������ �������� �ִܰŸ�
	float Distance(const Vector3& point) const;
	// ������ ���� ����� ����/�ܺ����� �Ǻ�
	bool IsOutside(const Vector3& point) const;

	// ����� ������ ����ȭ
	void Normalize();

	// �������κ��� �������� �ִ� �Ÿ��� ����
	float D = 0.f;
	// ���� ���� : ����� �ո� ���� (����� ���ϴ� ����)
	Vector3 Normal = Vector3::UnitY;
};

}