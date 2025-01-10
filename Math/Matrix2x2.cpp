#include "pch.h"
#include "Matrix2x2.h"

namespace JE {

const Matrix2x2 Matrix2x2::Identity = { Vector2::UnitX, Vector2::UnitY };

const Vector2& Matrix2x2::operator[](uint8 index) const
{
    return matrix[index];
}

Vector2& Matrix2x2::operator[](uint8 index)
{
    return matrix[index];
}

Matrix2x2 Matrix2x2::operator+(const Matrix2x2& m) const
{
    return Matrix2x2(
        matrix[0] + m[0],
        matrix[1] + m[1]
    );
}

Matrix2x2 Matrix2x2::operator*(const float scalar) const
{
    return Matrix2x2(
        matrix[0] * scalar,
        matrix[1] * scalar
    );
}

Matrix2x2 Matrix2x2::operator*(const Matrix2x2& m) const
{
    // 내적 연산 활용하기 위해 전치
    Matrix2x2 transposeM = m.Transpose();
    return Matrix2x2(
        Vector2(matrix[0].Dot(transposeM[0]), matrix[0].Dot(transposeM[1])),
        Vector2(matrix[1].Dot(transposeM[0]), matrix[1].Dot(transposeM[1]))
    );
}

Matrix2x2& Matrix2x2::operator+=(const Matrix2x2& m)
{
    matrix[0] += m[0];
    matrix[1] += m[1];
    return *this;
}

Matrix2x2& Matrix2x2::operator*=(const float scalar)
{
    matrix[0] *= scalar;
    matrix[1] *= scalar;
    return *this;
}

Matrix2x2& Matrix2x2::operator*=(const Matrix2x2& m)
{
    *this = *this * m;
    return *this;
}

Matrix2x2 Matrix2x2::Transpose() const
{
    return Matrix2x2(
        Vector2(matrix[0].X, matrix[1].X),
        Vector2(matrix[0].Y, matrix[1].Y)
    );
}

Matrix2x2 Matrix2x2::ScaleTransform(const float xScale, const float yScale)
{
    return Matrix2x2(
        Vector2(xScale, 0.f),
        Vector2(0.f, yScale)
    );
}

Matrix2x2 Matrix2x2::RotationTransform(const float degree)
{
    // 각도에 해당하는 sin, cos 값 계산
    float sin = 0.f, cos = 0.f;
    MathUtils::GetSinCos(sin, cos, degree);
    return Matrix2x2(
        Vector2(cos, sin),
        Vector2(-sin, cos)
    );
}

Matrix2x2 Matrix2x2::ShearTransformX(const float x)
{
    return Matrix2x2(
        Vector2::UnitX,
        Vector2(x, 1.f)
    );
}

Matrix2x2 Matrix2x2::ShearTransformY(const float y)
{
    return Matrix2x2(
        Vector2(1.f, y),
        Vector2::UnitY
    );
}

Matrix2x2 Matrix2x2::InverseScaleTransform(const Matrix2x2& m)
{
    return Matrix2x2(
        Vector2(1.f / m[0].X, 0.f),
        Vector2(0.f, 1.f / m[1].Y)
    );
}

Matrix2x2 Matrix2x2::InverseRotationTransform(const Matrix2x2& m)
{
    return m.Transpose();
}

Matrix2x2 Matrix2x2::InverseShereTransformX(const Matrix2x2& m)
{
    return Matrix2x2(
        Vector2::UnitX,
        Vector2(-m[1].X, 1.f)
    );
}

Matrix2x2 Matrix2x2::InverseShereTransformY(const Matrix2x2& m)
{
    return Matrix2x2(
        Vector2(1.f, -m[0].Y),
        Vector2::UnitY
    );
}

std::vector<std::wstring> Matrix2x2::ToString() const
{
    std::vector<std::wstring> result;
    for (uint8 i = 0; i < 2; ++i) 
        result.emplace_back(std::format(L"| {:.3f}, {:.3f} |", matrix[i].X, matrix[i].Y));
    
    return result;
}
}