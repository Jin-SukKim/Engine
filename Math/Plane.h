#pragma once

namespace JE {

struct Plane
{
	Plane() = default;
	// 평면은 세 점 또는 법선 벡터와 평면 위의 한 점으로 정의
	Plane(const Vector3& normal, const float& d);
	Plane(const Vector3& normal, const Vector3 planePt);
	// 평면을 구성하는 세점
	Plane(const Vector3& pos1, const Vector3& pos2, const Vector3& pos3);
	// 평면을 구성하는 a, b, c, d가 Vector4로 주어질 때 (평면의 방정식 : ax + by + cz + d = 0)
	Plane(const Vector4& v);

	// point < 0 : 평면의 안쪽, point == 0 : 평면의 위, 0 < point : 평면의 바깥, 절대값은 평면에서 임의의 점까지의 최단거리
	float Distance(const Vector3& point) const;
	// 임의의 점이 평면의 내부/외부인지 판별
	bool IsOutside(const Vector3& point) const;

	// 평면의 방정식 정규화
	void Normalize();

	// 원점으로부터 평면까지의 최단 거리와 방향
	float D = 0.f;
	// 법선 벡터 : 평면의 앞면 방향 (평면이 향하는 방향)
	Vector3 Normal = Vector3::UnitY;
};

}