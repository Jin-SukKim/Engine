#pragma once

namespace JE {

struct MathUtils
{
	static constexpr float PI = { 3.14159265358979323846f };
	static constexpr float TwoPI = { 2.f * PI };
	static constexpr float HalfPI = { 1.57079632679f };
	static constexpr float InvPI = { 0.31830988618f };

	// ����
	template<typename T>
	__forceinline static const int TruncToInt(float num) {
		return static_cast<T>(num);
	}
	// �ݿø�
	template<typename T>
	__forceinline static const int RoundToInt(float num) {
		return TruncToInt<T>(std::roundf(num));
	}
	// ����
	template<typename T>
	__forceinline static const int FloorToInt(float num) {
		return TruncToInt<T>(std::floorf(num));
	}
	// ����
	template<typename T>
	__forceinline static const int CeilToInt(float num) {
		return TruncToInt<T>(std::ceilf(num));
	}

	// �ε��Ҽ����� ����Ȯ�ϹǷ� ���� ���� ���� ������ ���� ���ٰ� �Ǵ�
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

	// ������ Radian ������ ���� �� �ﰢ�Լ� sin, cos ���� ���� ����
	__forceinline static const void GetSinCosRad(float& outSin, float& outCos, float radian);
	// ������ Degree ������ ���� �� �ﰢ�Լ� sin, cos ���� ���� ����
	__forceinline static const void GetSinCos(float& outSin, float& outCos, float degree);
};

}