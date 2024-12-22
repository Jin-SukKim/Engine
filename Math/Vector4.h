#pragma once

namespace JE {
	
struct Matrix4x4;

struct Vector4
{
	Vector4() : X(0.f), Y(0.f), Z(0.f), W(0.f) {}
	Vector4(const Vector2& v, bool isPoint = true) : X(v.X), Y(v.Y), Z(0.f), W(isPoint ? 1.f : 0.f) {}
	Vector4(const Vector3& v, bool isPoint = true) : X(v.X), Y(v.Y), Z(v.Z), W(isPoint ? 1.f : 0.f) {}
	Vector4(const Vector3& v, float w) : X(v.X), Y(v.Y), Z(v.Z), W(w) {}
	Vector4(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) {}
	Vector4(float x, float y, float z, bool isPoint = true) : X(x), Y(y), Z(z), W(isPoint ? 1.f : 0.f) {}

	float operator[](uint8 index) const;
	float& operator[](uint8 index);
	Vector4 operator-() const;

	Vector4 operator+(const Vector4& v) const;
	Vector4 operator-(const Vector4& v) const;
	Vector4 operator*(const float& scalar) const;
	Vector4 operator*(const Vector4& v) const;
	Vector4 operator/(const float& scalar) const;
	Vector4 operator*(const Matrix4x4& m) const;
	Vector4& operator+=(const Vector4& v);
	Vector4& operator-=(const Vector4& v);
	Vector4& operator*=(const float& scalar);
	Vector4& operator*=(const Vector4& v);
	Vector4& operator/=(const float& scalar);
	Vector4& operator*=(const Matrix4x4& m);

	bool operator==(const Vector4& v);
	bool Equal(const Vector4& v, float Tolerance = SMALL_NUMBER);

	float Size();
	float SizeSquared() const;

	Vector4 GetNormalize();
	void Normalize();

	float Dot(const Vector4& v) const;

	Vector2 ToVector2() const;
	Vector3 ToVector3() const;

	std::wstring ToString() const;

	static const Vector4 UnitX;
	static const Vector4 UnitY;
	static const Vector4 UnitZ;
	static const Vector4 UnitW;
	static const Vector4 One;
	static const Vector4 Zero;

	union {
		struct {
			float X, Y, Z, W;
		};
		std::array<float, static_cast<uint8>(4)> Scalars = { 0.f, 0.f, 0.f, 0.f };
	};
};

};
