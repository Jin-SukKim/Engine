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

	// 뷰 공간의 점이 절두체에 속해 있는지를 판별 (구분 : 외부, 겹칠때, 내부)
	BoundCheckResult CheckBound(const Vector3& point) const;
	// 구 바운딩 볼륨을 사용한 절두체 컬링
	BoundCheckResult CheckBound(const Sphere& sphere) const;
	// 박스 바운딩 볼륨(AABB)을 사용한 절두체 컬링
	BoundCheckResult CheckBound(const Box& box) const;

	// 평면의 방정식을 활용한 절두체
	// Y-(좌측), Y+(우측), X-(하단), X+(상단), Z-(근평면), Z+(원평면) 순으로 저장
	std::array<Plane, 6> Planes;
};

}
