#pragma once

namespace JE {

/*
	q = w + xi + yj + zk = (w, v), 
	w = 실수, v = (x, y, z) 허수부

	내부적으로 사원수를 사용한 회전으로 자유로운 회전 구현 (Rotator -> Quaternion -> 회전 행렬 -> 렌더링)
	실수부가 0인 사원수를 순허수 사원수(Pure Imaginary Quaternion)이라 부르면 세 허수로 구성되기에 3차원 벡터 공간에 대응
*/ 
struct Quaternion
{
	Quaternion() = default;
	// v = (X, Y, Z) 허수부, W = 실수
	Quaternion(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) {}
	// 회전출 n과 회전할 각도로 회전 사원수 생성
	Quaternion(const Vector3& axis, float angleDegree);
	// 오일러 각(Rotator)으로부터 사원수 생성
	Quaternion(const Rotator& rotator);
	// 회전 변환행렬로부터 사원수 생성
	Quaternion(const Matrix3x3& mat);

	Quaternion operator-() const;
	Quaternion operator+(const Quaternion& q) const;
	Quaternion operator-(const Quaternion& q) const;
	Quaternion operator+=(const Quaternion& q);
	Quaternion operator-=(const Quaternion& q);
	// 사원수의 곱셈 (해밀턴 곱)
	Quaternion operator*(const Quaternion& q) const;
	Quaternion operator*=(const Quaternion& q);

	// 사원수의 크기 (Norm)
	float NormSize() const;
	// 정규화
	void Normalize();
	// 크기가 1인 단위 사원수
	bool IsUnitQuaternion() const;

	// 켤레 사원수 : q* = w - (xi + yj + zk) = (w, -v), 단위 사원수인 경우 반대 방향 회전
	Quaternion Conjugate() const;

	// 단위 사원수인 경우 역원은 켤레 사원수와 동일 (이 엔진에서는 단위 사원수의 역원을 찾는다고 가정)
	Quaternion Inverse() const;

	// 회전 사원수 q(순허수 사원수)를 사용해 임의의 회전축을 중심으로 각도만큼 회전 (축-각 회전에 대응)
	Vector3 RotateVector(const Vector3& v) const;
	// 회전 사원수로 벡터를 회전
	Vector3 operator*(const Vector3& v) const;

	// 오일러 각에서 사원수로 변환해 사원수 생성
	void FromRotator(const Rotator& rotator);

	// 사원수에서 오일러 각으로 변환
	Rotator ToRotator() const;

	// 로컬축 X을 회전 사원수로 회전
	static Vector3 GetLocalX(const Quaternion& q) { return q * Vector3::UnitX; }
	// 로컬축 Y을 회전 사원수로 회전
	static Vector3 GetLocalY(const Quaternion& q) { return q * Vector3::UnitY; }
	// 로컬축 Z을 회전 사원수로 회전
	static Vector3 GetLocalZ(const Quaternion& q) { return q * Vector3::UnitZ; }

	// 사원수에서 회전 변환행렬로 변환 (사원수로 회전된 세 로컬축 벡터를 사용해 회전 변환행렬 생성)
	static Matrix4x4 GetRotationTransform(const Quaternion& q);

	// 회전 변환행렬에서 사원수로 변환
	void FromMatrix(const Matrix3x3& mat);

	// 사원수의 구면 선형 보간 (임의의 축에 대한 회전 보간) - 각도를 이용해 보간해 부드러운 회전
	static Quaternion Slerp(const Quaternion& quat1, const Quaternion& quat2, float ratio);

	// 사원수의 실수부 W
	float RealPart() const { return W; }
	// 사원수의 허수부, 3차원 벡터 v
	Vector3 ImaginaryPart() const { return Vector3(X, Y, Z); }

	// 사원수를 이해하기 편한 오일러 각으로 변환한 뒤 출력
	std::wstring ToString() const;

	static const Quaternion Identity;

	// 허수부
	float X = 0.f;
	float Y = 0.f;
	float Z = 0.f;
	// 실수부
	float W = 1.f;
};

}
