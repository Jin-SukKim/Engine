#pragma once

namespace JE {

struct Matrix3x3
{
	Matrix3x3() = default;
	Matrix3x3(const Vector3& row0, const Vector3& row1, const Vector3& row2) { matrix = { row0, row1, row2 }; }

	const Vector3& operator[](uint8 index) const;
	Vector3& operator[](uint8 index);

	Matrix3x3 operator+(const Matrix3x3& m) const;
	Matrix3x3 operator*(const float scalar) const;
	Matrix3x3 operator*(const Matrix3x3& m) const;

	Matrix3x3& operator+=(const Matrix3x3& m);
	Matrix3x3& operator*=(const float scalar);
	Matrix3x3& operator*=(const Matrix3x3& m);

	Matrix3x3 Transpose() const;

	// 변환행렬
	static Matrix3x3 TranslationTransform(const Vector2& v);
	static Matrix3x3 ScaleTransform(const float scale);
	static Matrix3x3 ScaleTransform(const float xScale, const float yScale);
	static Matrix3x3 ScaleTransform(const Vector2& scale);
	static Matrix3x3 RotationTransform(const float degree);

	// 역행렬
	static Matrix3x3 InverseTranslationTransform(const Matrix3x3& m);
	static Matrix3x3 InverseScaleTransform(const Matrix3x3& m);
	static Matrix3x3 InverseRotationTransform(const Matrix3x3& m);

	std::vector<std::wstring> ToString() const;

	static const Matrix3x3 Identity;

	std::array<Vector3, static_cast<uint8>(3)> matrix = { Vector3::UnitX, Vector3::UnitY, Vector3::UnitZ };
};
}
