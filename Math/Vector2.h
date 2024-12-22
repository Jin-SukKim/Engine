#pragma once

namespace JE {

struct Matrix2x2;
struct Matrix3x3;

struct Vector2
{
	Vector2() : X(0.f), Y(0.f) {}
	Vector2(float scalar) : X(scalar), Y(scalar) {}
	Vector2(float x, float y) : X(x), Y(y) {}
	Vector2(int x, int y) : X(static_cast<float>(x)), Y(static_cast<float>(y)) {}
	Vector2(const Vector2& vect) : X(vect.X), Y(vect.Y) {}

	float operator[](uint8 index) const;
	float& operator[](uint8 index);
	Vector2 operator-() const; // 벡터의 부호 반전

	// 연산자 오버로딩
	Vector2 operator+(const Vector2& v) const;
	Vector2 operator-(const Vector2& v) const;
	Vector2 operator*(const Vector2& v) const;
	Vector2 operator*(const float& scalar) const;
	Vector2 operator/(const float& scalar) const;
	Vector2& operator+=(const Vector2& v);
	Vector2& operator-=(const Vector2& v);
	Vector2& operator*=(const Vector2& v);
	Vector2& operator*=(const float& scalar);
	Vector2& operator/=(const float& scalar);

	Vector2 operator*(const Matrix2x2& m) const;
	Vector2 operator*(const Matrix3x3& m) const;
	Vector2& operator*=(const Matrix2x2& m);
	Vector2& operator*=(const Matrix3x3& m);

	bool operator==(const Vector2& v);
	// 허용 오차 범위 = SMALL_NUMBER
	bool Equal(const Vector2& v, float Tolerance = SMALL_NUMBER);

	float Size() const;
	float SizeSquared() const; // sqrt 연산은 비용이 크므로 제곱값을 사용하는 경우가 많음

	// 정규화 : 크기가 1인 단위 벡터 (Unit Vector)
	Vector2 GetNormalize() const;
	// 현재 벡터를 정규화
	void Normalize();

	// 삼각함수를 활용한 2D 회전 (angle = degree)
	Vector2 Rotate2D(const float& angle) const;

	// 벡터가 x축과 이루는 (-180, 180) 범위의 사이각 
	float Angle() const; // Radian
	float AngleToDegree() const; // Degree

	// 회전에 편리한 극좌표계로 변환 (극좌표계의 각의 범위 [-PI, PI]) : 극좌표계 = (Size, Angle)
	Vector2 ToPolarCoordinate() const;
	// 극좌표계에서 데카르트 좌표계로 변환
	Vector2 ToCartesianCoordinate() const;
	 
	/*
		내적 = |u||v|cos각도 : 두 벡터의 사이각에 대한 cos함수와 비례, 두 벡터가 단위 벡터면 사이각
		내적의 값이 0이면 cos 90, 270(-90)도로 두 벡터가 직교하는 경우
		내적은 각도 (-90, 90) 영역에 양수, -90과 90도에 0(직교), 나머지 범위에서 음수
	*/  
	float Dot(const Vector2& v) const;
	
	// 벡터 v에 투영
	Vector2 Projection(const Vector2& v) const;

	// print
	std::wstring ToString() const;

	// 미리 선언해 둔 벡터 값들
	static const Vector2 UnitX;
	static const Vector2 UnitY;
	static const Vector2 One;
	static const Vector2 Zero;

	union {
		struct {
			float X, Y;
		};
		std::array<float, static_cast<uint8>(2)> Scalars = { 0.f, 0.f };
	};
};

}