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

		// 법선 벡터
		Normal = v1.Cross(v2).GetNormalize();
		// D 값 계산
		D = -Normal.Dot(pos1);
	}

	Plane::Plane(const Vector4& v)
	{
		// 평면의 방정식 : ax + by + cz + d = 0
		// 평면의 세 요소 a, b, c를 법선 벡터에 지정
		Normal = v.ToVector3();
		// 마지막 요소를 D에 지정
		D = v.W;
		// 네 요소를 모두 정규화 (법선 벡터 크기를 1로 변경 동시에 D도 같이 변경
		// D도 정규화 해줘야 원점으로부터의 최단 거리 정보를 담게 됨
		Normalize();
	}

	float Plane::Distance(const Vector3& point) const
	{
		// 임의의 점을 법선 벡터에 투영시킨 후 D를 더해줌
		return Normal.Dot(point) + D;
	}

	bool Plane::IsOutside(const Vector3& point) const
	{
		// Distance() > 0 : 평면의 바깥쪽
		// Distance() == 0  : 평면 위
		// Distance() < 0 : 평면의 안쪽
		return Distance(point) > 0.f;
	}

	void Plane::Normalize()
	{
		float squaredSize = Normal.SizeSquared();
		if (MathUtils::EqualsInTolerance(squaredSize, 1.f))
			return;


		// 정규화
		 float invLength = 1.f / std::sqrtf(squaredSize); // TODO: Fast sqrt
		Normal *= invLength;
		D *= invLength;
	}
}