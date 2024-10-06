#pragma once

namespace JE {

struct MathUtils
{
	static constexpr float PI = { 3.14159265358979323846f };
	static constexpr float TwoPI = { 2.f * PI };
	static constexpr float HalfPI = { 1.57079632679f };
	static constexpr float InvPI = { 0.31830988618f };

	// 버림
	template<typename T>
	__forceinline static const int TruncToInt(float num) {
		return static_cast<T>(num);
	}
	// 반올림
	template<typename T>
	__forceinline static const int RoundToInt(float num) {
		return TruncToInt<T>(std::roundf(num));
	}
	// 버림
	template<typename T>
	__forceinline static const int FloorToInt(float num) {
		return TruncToInt<T>(std::floorf(num));
	}
	// 높임
	template<typename T>
	__forceinline static const int CeilToInt(float num) {
		return TruncToInt<T>(std::ceilf(num));
	}

	// 부동소수점은 부정확하므로 아주 작은 오차 범위의 수는 같다고 판단
	__forceinline static const bool EqualsInTolerance(float num1, float num2, float tolerance = SMALL_NUMBER) {
		return std::abs(num1 - num2) <= tolerance;
	}

	// Degree => Radian
	__forceinline static const float Deg2Rad(float degree) {
		return degree * PI / 180.f;
	}

	// Radian => Degree
	__forceinline static const float Rad2Deg(float radian) {
		return radian * 180.f * InvPI;
	}

	// 지정된 Radian 각도에 대한 두 삼각함수 sin, cos 값을 구해 대입
	__forceinline static const void GetSinCosRad(float& outSin, float& outCos, float radian);
	// 지정된 Degree 각도에 대한 두 삼각함수 sin, cos 값을 구해 대입
	__forceinline static const void GetSinCos(float& outSin, float& outCos, float degree);
};

}