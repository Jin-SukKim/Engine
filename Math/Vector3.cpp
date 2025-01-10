#include "pch.h"
#include "Vector3.h"
#include "Matrix3x3.h"

namespace JE {
	const Vector3 Vector3::UnitX = { 1.f, 0.f, 0.f };
	const Vector3 Vector3::UnitY = { 0.f, 1.f, 0.f };
	const Vector3 Vector3::UnitZ = { 0.f, 0.f, 1.f };
	const Vector3 Vector3::One = { 1.f, 1.f, 1.f };
	const Vector3 Vector3::Zero = { 0.f, 0.f, 0.f };


	float Vector3::operator[](uint8 index) const
	{
		return Scalars[index];
	}
	float& Vector3::operator[](uint8 index)
	{
		return Scalars[index];
	}
	Vector3 Vector3::operator-() const
	{
		return Vector3(-X, -Y, -Z);
	}
	Vector3 Vector3::operator+(const Vector3& v) const
	{
		return Vector3(X + v.X, Y + v.Y, Z + v.Z);
	}
	Vector3 Vector3::operator-(const Vector3& v) const
	{
		return Vector3(X - v.X, Y - v.Y, Z - v.Z);
	}
	Vector3 Vector3::operator*(const float& scalar) const
	{
		return Vector3(X * scalar, Y * scalar, Z * scalar);
	}
	Vector3 Vector3::operator*(const Vector3& v) const
	{
		return Vector3(X * v.X, Y * v.Y, Z * v.Z);
	}
	Vector3 Vector3::operator/(const float& scalar) const
	{
		return Vector3(X / scalar, Y / scalar, Z / scalar);
	}
	Vector3& Vector3::operator+=(const Vector3& v)
	{
		X += v.X;
		Y += v.Y;
		Z += v.Z;
		return *this;
	}

	Vector3& Vector3::operator-=(const Vector3& v)
	{
		X -= v.X;
		Y -= v.Y;
		Z -= v.Y;
		return *this;
	}

	Vector3& Vector3::operator*=(const float& scalar)
	{
		X *= scalar;
		Y *= scalar;
		Z *= scalar;
		return *this;
	}

	Vector3& Vector3::operator*=(const Vector3& v)
	{
		X *= v.X;
		Y *= v.Y;
		Z *= v.Z;
		return *this;
	}

	Vector3& Vector3::operator/=(const float& scalar)
	{
		X /= scalar;
		Y /= scalar;
		Z /= scalar;
		return *this;
	}

	Vector3 Vector3::operator*(const Matrix3x3& m) const
	{
		// Dot을 사용하기 위해 transpose 시킨 후 계산
		Matrix3x3 transposedM = m.Transpose();
		return Vector3(
			this->Dot(transposedM[0]), this->Dot(transposedM[1]), this->Dot(transposedM[2])
		);
	}

	Vector3& Vector3::operator*=(const Matrix3x3& m)
	{
		*this = *this * m;
		return *this;
	}

	bool Vector3::operator==(const Vector3& v)
	{
		return this->Equal(v);
	}

	bool Vector3::Equal(const Vector3& v, float Tolerance)
	{
		return std::abs(X - v.X) <= Tolerance &&
			std::abs(Y - v.Y) <= Tolerance &&
			std::abs(Z - v.Z) <= Tolerance;
	}
	float Vector3::Size() const
	{
		return std::sqrtf(this->SizeSquared());
	}
	float Vector3::SizeSquared() const
	{
		return X * X + Y * Y + Z * Z;
	}
	float Vector3::Max() const
	{
		return std::max({ X, Y, Z });
	}
	Vector3 Vector3::GetNormalize() const
	{
		float squareSize = this->SizeSquared();
		if (squareSize == 1.f)
			return *this;

		if (squareSize == 0.f)
			return Vector3::Zero;

		// TODO: fast-sqrt 사용하기
		return *this / std::sqrtf(squareSize);
	}
	void Vector3::Normalize()
	{
		*this = this->GetNormalize();
	}
	float Vector3::Dot(const Vector3& v) const
	{
		return X * v.X + Y * v.Y + Z * v.Z;
	}
	Vector3 Vector3::Projection(const Vector3& v) const
	{
		Vector3 unitV = v.GetNormalize(); // 정규화
		// 내적으로 투영된 길이를 구한 뒤 방향벡터를 곱해 투영벡터 생성
		return unitV * this->Dot(unitV);
	}
	Vector3 Vector3::Cross(const Vector3& v) const
	{
		return Vector3(
			Y * v.Z - v.Y * Z,
			Z * v.X - v.Z * X,
			X * v.Y - v.X * Y
		);
	}
	Vector2 Vector3::ToVector2() const
	{
		return Vector2(X, Y);
	}
	std::wstring Vector3::ToString() const
	{
		return std::format(L"{:3f}, {:.3f}, {:.3f}", X, Y, Z);
	}
}