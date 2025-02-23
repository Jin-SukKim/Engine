#include "pch.h"
#include "Matrix4x4.h"

namespace JE {
	const Matrix4x4 Matrix4x4::Identity = { Vector4::UnitX, Vector4::UnitY, Vector4::UnitZ, Vector4::UnitW };
	const Vector4& Matrix4x4::operator[](uint8 index) const
	{
		return matrix[index];
	}
	Vector4& Matrix4x4::operator[](uint8 index)
	{
		return matrix[index];
	}
	Matrix4x4 Matrix4x4::operator+(const Matrix4x4& m) const
	{
		return Matrix4x4(
			matrix[0] + m[0],
			matrix[1] + m[1],
			matrix[2] + m[2],
			matrix[3] + m[3]
		);
	}
	Matrix4x4 Matrix4x4::operator*(const float scalar) const
	{
		return Matrix4x4(
			matrix[0] * scalar,
			matrix[1] * scalar,
			matrix[2] * scalar,
			matrix[3] * scalar
		);
	}
	Matrix4x4 Matrix4x4::operator*(const Matrix4x4& m) const
	{
		Matrix4x4 transposedM = m.Transpose();
		return Matrix4x4(
			Vector4(matrix[0].Dot(transposedM[0]), matrix[0].Dot(transposedM[1]), matrix[0].Dot(transposedM[2]), matrix[0].Dot(transposedM[3])),
			Vector4(matrix[1].Dot(transposedM[0]), matrix[1].Dot(transposedM[1]), matrix[1].Dot(transposedM[2]), matrix[1].Dot(transposedM[3])),
			Vector4(matrix[2].Dot(transposedM[0]), matrix[2].Dot(transposedM[1]), matrix[2].Dot(transposedM[2]), matrix[2].Dot(transposedM[3])),
			Vector4(matrix[3].Dot(transposedM[0]), matrix[3].Dot(transposedM[1]), matrix[3].Dot(transposedM[2]), matrix[3].Dot(transposedM[3]))
		);
	}
	Matrix4x4& Matrix4x4::operator+=(const Matrix4x4& m)
	{
		matrix[0] += m[0];
		matrix[1] += m[1];
		matrix[2] += m[2];
		matrix[3] += m[3];
		return *this;
	}
	Matrix4x4& Matrix4x4::operator*=(const float scalar)
	{
		matrix[0] *= scalar;
		matrix[1] *= scalar;
		matrix[2] *= scalar;
		matrix[3] *= scalar;
		return *this;
	}
	Matrix4x4& Matrix4x4::operator*=(const Matrix4x4& m)
	{
		*this = *this * m;
		return *this;
	}
	Matrix4x4 Matrix4x4::Transpose() const
	{
		return Matrix4x4(
			Vector4(matrix[0].X, matrix[1].X, matrix[2].X, matrix[3].X),
			Vector4(matrix[0].Y, matrix[1].Y, matrix[2].Y, matrix[3].Y),
			Vector4(matrix[0].Z, matrix[1].Z, matrix[2].Z, matrix[3].Z),
			Vector4(matrix[0].W, matrix[1].W, matrix[2].W, matrix[3].W)
		);
	}
	Matrix4x4 Matrix4x4::TranslationTransform(const Vector3& v)
	{
		return Matrix4x4(
			Vector4::UnitX,
			Vector4::UnitY,
			Vector4::UnitZ,
			Vector4(v, 1.f)
		);
	}
	Matrix4x4 Matrix4x4::ScaleTransform(const float scale)
	{
		return Matrix4x4(
			Vector4(scale, 0.f, 0.f, 0.f),
			Vector4(0.f, scale, 0.f, 0.f),
			Vector4(0.f, 0.f, scale, 0.f),
			Vector4::UnitW
		);
	}
	Matrix4x4 Matrix4x4::ScaleTransform(const Vector3& scale)
	{
		return Matrix4x4(
			Vector4(scale.X, 0.f, 0.f, 0.f),
			Vector4(0.f, scale.Y, 0.f, 0.f),
			Vector4(0.f, 0.f, scale.Z, 0.f),
			Vector4::UnitW
		);
	}
	Matrix4x4 Matrix4x4::RotationTransform(const Vector3& right, const Vector3& up, const Vector3& forward)
	{
		return Matrix4x4(
			Vector4(right, 0.f),
			Vector4(up, 0.f),
			Vector4(forward, 0.f),
			Vector4::UnitW
		);
	}
	Matrix4x4 Matrix4x4::InverseTranslationTransform(const Matrix4x4& m)
	{
		return Matrix4x4(
			Vector4::UnitX,
			Vector4::UnitY,
			Vector4::UnitZ,
			Vector4(-m[3].X, -m[3].Y, -m[3].Z, 1.f)
		);
	}
	Matrix4x4 Matrix4x4::InverseScaleTransform(const Matrix4x4& m)
	{
		return Matrix4x4(
			Vector4(1.f / m[0].X, 0.f, 0.f, 0.f),
			Vector4(0.f, 1.f / m[1].Y, 0.f, 0.f),
			Vector4(0.f, 0.f, 1.f / m[2].Z, 0.f),
			Vector4::UnitW
		);
	}
	Matrix4x4 Matrix4x4::InversRotationTransform(const Matrix4x4& m)
	{
		return m.Transpose();
	}
	std::vector<std::wstring> Matrix4x4::ToString() const
	{
		std::vector<std::wstring> s;
		for (uint8 i = 0; i < 3; ++i)
			s.emplace_back(std::format(L"| {:.3f}, {:.3f}, {:.3f}, {:.3f} |", matrix[i].X, matrix[i].Y, matrix[i].Z, matrix[i].W));
		return s;
	}
}