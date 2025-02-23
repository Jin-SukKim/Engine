#include "pch.h"
#include "Plane.h"

namespace JE {
	Plane::Plane(const Vector3& normal, const float& d) : Normal(normal), D(d) { 
		if (Normal.SizeSquared() != 1.f) Normal.Normalize(); 
	}

	Plane::Plane(const Vector3& normal, const Vector3 planePt) : Normal(normal) {
		if (Normal.SizeSquared() != 1.f)
			Normal.Normalize();
		D = -Normal.Dot(planePt);
	}

	Plane::Plane(const Vector3& pos1, const Vector3& pos2, const Vector3& pos3)
	{
		Vector3 v1 = pos2 - pos1;
		Vector3 v2 = pos3 - pos1;

		// ���� ����
		Normal = v1.Cross(v2).GetNormalize();
		// D �� ���
		D = -Normal.Dot(pos1);
	}

	Plane::Plane(const Vector4& v)
	{
		// ����� ������ : ax + by + cz + d = 0
		// ����� �� ��� a, b, c�� ���� ���Ϳ� ����
		Normal = v.ToVector3();
		// ������ ��Ҹ� D�� ����
		D = v.W;
		// �� ��Ҹ� ��� ����ȭ (���� ���� ũ�⸦ 1�� ���� ���ÿ� D�� ���� ����
		// D�� ����ȭ ����� �������κ����� �ִ� �Ÿ� ������ ��� ��
		Normalize();
	}

	float Plane::Distance(const Vector3& point) const
	{
		// ������ ���� ���� ���Ϳ� ������Ų �� D�� ������
		return Normal.Dot(point) + D;
	}

	bool Plane::IsOutside(const Vector3& point) const
	{
		// Distance() > 0 : ����� �ٱ���
		// Distance() == 0  : ��� ��
		// Distance() < 0 : ����� ����
		return Distance(point) > 0.f;
	}

	void Plane::Normalize()
	{
		float squaredSize = Normal.SizeSquared();
		if (MathUtils::EqualsInTolerance(squaredSize, 1.f))
			return;


		// ����ȭ
		 float invLength = 1.f / std::sqrtf(squaredSize); // TODO: Fast sqrt
		Normal *= invLength;
		D *= invLength;
	}
}