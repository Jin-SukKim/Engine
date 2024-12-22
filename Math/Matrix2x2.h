#pragma once

namespace JE {

struct Matrix2x2
{
	Matrix2x2() = default;
	Matrix2x2(const Vector2& row0, const Vector2& row1) { matrix = { row0, row1 }; }

	const Vector2& operator[](uint8 index) const;
	Vector2& operator[](uint8 index);

	Matrix2x2 operator+(const Matrix2x2& m) const;
	Matrix2x2 operator*(const float scalar) const;
	Matrix2x2 operator*(const Matrix2x2& m) const;

	Matrix2x2& operator+=(const Matrix2x2& m);
	Matrix2x2& operator*=(const float scalar);
	Matrix2x2& operator*=(const Matrix2x2& m);

	// ÀüÄ¡
	Matrix2x2 Transpose() const;
	
	static Matrix2x2 ScaleTransform(const float xScale, const float yScale);
	static Matrix2x2 RotationTransform(const float degree);
	static Matrix2x2 ShearTransformX(const float x);
	static Matrix2x2 ShearTransformY(const float y);
	
	static Matrix2x2 InverseScaleTransform(const Matrix2x2& m);
	static Matrix2x2 InverseRotationTransform(const Matrix2x2& m);
	static Matrix2x2 InverseShereTransformX(const Matrix2x2& m);
	static Matrix2x2 InverseShereTransformY(const Matrix2x2& m);

	std::vector<std::wstring> ToString() const;

	static const Matrix2x2 Identity;
	// Row-Major
	std::array<Vector2, static_cast<uint8>(2)> matrix = { Vector2::UnitX, Vector2::UnitY };
};

};
