#pragma once

namespace JE {

/*
	오일러 각을 사용하는 회전
	Yaw = y축 회전 (좌우), Roll = Z축 회전(회전), Pitch = X축 회전(위아래)
*/ 
struct Rotator
{
	Rotator() = default;
	Rotator(float yaw, float roll, float pitch) : Yaw(yaw), Roll(roll), Pitch(pitch) {}

	// 오일러 각의 회전값을 사용하기 편리하게 재구성, 각도 값을 [0, 360] 범위로 변경
	void Clamp();

	// 각도 값을 [0, 360] 범위로 변경
	float GetAxisClampValue(float value);

	// 회전 변환된 로컬 축 (Roll -> Pitch -> Yaw 회전 사용)
	void GetLocalAxes(Vector3& outRight, Vector3& outUp, Vector3& outForward);

	std::wstring ToString() const;

	static const Rotator Identity;

	// 축을 기준으로 회전 (Degree)
	float Yaw = 0.f; // Y축 회전
	float Roll = 0.f; // Z축 회전
	float Pitch = 0.f; // X축 회전
};


}