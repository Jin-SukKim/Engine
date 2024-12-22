#pragma once

namespace JE {
	
struct Matrix4x4
{
	Matrix4x4() = default;
	Matrix4x4(const Vector4& row0, const Vector4& row1, const Vector4& row2, const Vector4& row3) { matrix = { row0, row1, row2, row3 }; }
	Matrix4x4(const Matrix3x3& m) { matrix = { m[0], m[1], m[2], Vector4::UnitW }; }
	
	const Vector4& operator[](uint8 index) const;
	Vector4& operator[](uint8 index);

	Matrix4x4 operator+(const Matrix4x4& m) const;
	Matrix4x4 operator*(const float scalar) const;
	Matrix4x4 operator*(const Matrix4x4& m) const;
	Matrix4x4& operator+=(const Matrix4x4& m);
	Matrix4x4& operator*=(const float scalar);
	Matrix4x4& operator*=(const Matrix4x4& m);

	Matrix4x4 Transpose() const;

	static Matrix4x4 TranslationTransform(const Vector3& v);
	static Matrix4x4 ScaleTransform(const float scale);
	static Matrix4x4 ScaleTransform(const Vector3& scale);
	// x√‡(right), y√‡(up), z√‡(forward)
	static Matrix4x4 RotationTransform(const Vector3& right, const Vector3& up, const Vector3& forward);

	static Matrix4x4 InverseTranslationTransform(const Matrix4x4& m);
	static Matrix4x4 InverseScaleTransform(const Matrix4x4& m);
	static Matrix4x4 InversRotationTransform(const Matrix4x4& m);

	std::vector<std::wstring> ToString() const;

	static const Matrix4x4 Identity;

	std::array<Vector4, static_cast<uint8>(4)> matrix = { Vector4::UnitX, Vector4::UnitY, Vector4::UnitZ, Vector4::UnitW };
};

}