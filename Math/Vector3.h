#pragma once

namespace JE {

struct Matrix3x3;

struct Vector3
{
	Vector3() : X(0.f), Y(0.f), Z(0.f) {}
	Vector3(float X, float Y, float Z) : X(X), Y(Y), Z(Z) {}
	Vector3(int X, int Y, int Z) : X(static_cast<float>(X)), Y(static_cast<float>(Y)), Z(static_cast<float>(Z)) {}
	Vector3(const Vector2& v, bool isPoint = true) : X(v.X), Y(v.Y), Z(isPoint ? 1.f : 0.f) {}
	Vector3(const Vector2& v, float z) : X(v.X), Y(v.Y), Z(z) {}
	Vector3(const Vector3& v) : X(v.X), Y(v.Y), Z(v.Z) {}

	float operator[](uint8 index) const;
	float& operator[](uint8 index);
	Vector3 operator-() const; // 부호 반전

	// 연산자
	Vector3 operator+(const Vector3& v) const;
	Vector3 operator-(const Vector3& v) const;
	Vector3 operator*(const float& scalar) const;
	Vector3 operator*(const Vector3& v) const;
	Vector3 operator/(const float& scalar) const;
	Vector3& operator+=(const Vector3& v);
	Vector3& operator-=(const Vector3& v);
	Vector3& operator*=(const float& scalar);
	Vector3& operator*=(const Vector3& v);
	Vector3& operator/=(const float& scalar);

	Vector3 operator*(const Matrix3x3& m) const;
	Vector3& operator*=(const Matrix3x3& m);

	bool operator==(const Vector3& v);

	// 허용오차 범위 안에서 같은지 확인
	bool Equal(const Vector3& v, float Tolerance = SMALL_NUMBER);

	float Size() const;
	float SizeSquared() const;

	// 벡터의 세 값 중 가장 큰 값
	float Max() const; 

	Vector3 GetNormalize() const;
	void Normalize();

	// 내적
	float Dot(const Vector3& v) const;

	// 벡터 v에 투영
	Vector3 Projection(const Vector3& v) const;

	// 외적
	Vector3 Cross(const Vector3& v) const;
	
	Vector2 ToVector2() const;
	std::wstring ToString() const;

	static const Vector3 UnitX;
	static const Vector3 UnitY;
	static const Vector3 UnitZ;
	static const Vector3 One;
	static const Vector3 Zero;

	union {
		struct {
			float X, Y, Z;
		};
		std::array<float, 3> Scalars = { 0.f, 0.f, 0.f };
	};
};
}
