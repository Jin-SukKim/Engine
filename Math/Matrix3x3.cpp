#include "pch.h"
#include "Matrix3x3.h"

namespace JE {

	const Matrix3x3 Matrix3x3::Identity = { Vector3::UnitX, Vector3::UnitY, Vector3::UnitZ };

	const Vector3& Matrix3x3::operator[](uint8 index) const
	{
		return matrix[index];
	}
	Vector3& Matrix3x3::operator[](uint8 index)
	{
		return matrix[index];
	}
	Matrix3x3 Matrix3x3::operator+(const Matrix3x3& m) const
	{
		return Matrix3x3(
			matrix[0] + m[0],
			matrix[1] + m[1],
			matrix[2] + m[2]
		);
	}
	Matrix3x3 Matrix3x3::operator*(const float scalar) const
	{
		return Matrix3x3(
			matrix[0] * scalar,
			matrix[1] * scalar,
			matrix[2] * scalar
		);
	}
	Matrix3x3 Matrix3x3::operator*(const Matrix3x3& m) const
	{
		Matrix3x3 transposeM = m.Transpose();
		return Matrix3x3(
			Vector3(matrix[0].Dot(transposeM[0]), matrix[0].Dot(transposeM[1]), matrix[0].Dot(transposeM[2])),
			Vector3(matrix[1].Dot(transposeM[0]), matrix[1].Dot(transposeM[1]), matrix[1].Dot(transposeM[2])),
			Vector3(matrix[2].Dot(transposeM[0]), matrix[2].Dot(transposeM[1]), matrix[2].Dot(transposeM[2]))
		);
	}
	Matrix3x3& Matrix3x3::operator+=(const Matrix3x3& m)
	{
		matrix[0] += m[0];
		matrix[1] += m[1];
		matrix[2] += m[2];
		return *this;
	}
	Matrix3x3& Matrix3x3::operator*=(const float scalar)
	{
		matrix[0] *= scalar;
		matrix[1] *= scalar;
		matrix[2] *= scalar;
		return *this;
	}
	Matrix3x3& Matrix3x3::operator*=(const Matrix3x3& m)
	{
		*this = *this * m;
		return *this;
	}
	Matrix3x3 Matrix3x3::Transpose() const
	{
		return Matrix3x3(
			Vector3(matrix[0].X, matrix[1].X, matrix[2].X),
			Vector3(matrix[0].Y, matrix[1].Y, matrix[2].Y),
			Vector3(matrix[0].Z, matrix[1].Z, matrix[2].Z)
		);
	}
	Matrix3x3 Matrix3x3::TranslationTransform(const Vector2& v)
	{
		return Matrix3x3(
			Vector3::UnitX,
			Vector3::UnitY,
			Vector3(v.X, v.Y, 1.f)
		);
	}
	Matrix3x3 Matrix3x3::ScaleTransform(const float scale)
	{
		return Matrix3x3(
			Vector3(scale, 0.f, 0.f),
			Vector3(0.f, scale, 0.f),
			Vector3::UnitZ
		);
	}
	Matrix3x3 Matrix3x3::ScaleTransform(const float xScale, const float yScale)
	{
		return Matrix3x3(
			Vector3(xScale, 0.f, 0.f),
			Vector3(0.f, yScale, 0.f),
			Vector3::UnitZ
		);
	}
	Matrix3x3 Matrix3x3::ScaleTransform(const Vector2& scale)
	{
		return Matrix3x3(
			Vector3(scale.X, 0.f, 0.f),
			Vector3(0.f, scale.Y, 0.f),
			Vector3::UnitZ
		);
	}
	Matrix3x3 Matrix3x3::RotationTransform(const float degree)
	{
		float sin = 0.f, cos = 0.f;
		MathUtils::GetSinCos(sin, cos, degree);
		return Matrix3x3(
			Vector3(cos, sin, 0.f),
			Vector3(-sin, cos, 0.f),
			Vector3::UnitZ
		);
	}
	Matrix3x3 Matrix3x3::InverseTranslationTransform(const Matrix3x3& m)
	{
		return Matrix3x3(
			Vector3::UnitX,
			Vector3::UnitY,
			Vector3(-m[2].X, -m[2].Y, 1.f)
		);
	}
	Matrix3x3 Matrix3x3::InverseScaleTransform(const Matrix3x3& m)
	{
		return Matrix3x3(
			Vector3(1.f / m[0].X, 0.f, 0.f),
			Vector3(0.f, 1.f / m[1].Y, 0.f),
			Vector3::UnitZ
		);
	}
	Matrix3x3 Matrix3x3::InverseRotationTransform(const Matrix3x3& m)
	{
		return m.Transpose();
	}
	std::vector<std::wstring> Matrix3x3::ToString() const
	{
		std::vector<std::wstring> s;
		for (uint8 i = 0; i < 3; ++i)
			s.emplace_back(std::format(L"| {:.3f}, {:.3f}, {:.3f} |", matrix[i].X, matrix[i].Y, matrix[i].Z));
		return s;
	}
};