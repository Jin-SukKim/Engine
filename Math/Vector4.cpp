#include "pch.h"
#include "Vector4.h"

namespace JE {

    const Vector4 Vector4::UnitX = { 1.f, 0.f, 0.f, 0.f };
    const Vector4 Vector4::UnitY = { 0.f, 1.f, 0.f, 0.f };
    const Vector4 Vector4::UnitZ = { 0.f, 0.f, 1.f, 0.f };
    const Vector4 Vector4::UnitW = { 0.f, 0.f, 0.f, 1.f };
    const Vector4 Vector4::One   = { 1.f, 1.f, 1.f, 1.f };
    const Vector4 Vector4::Zero  = { 0.f, 0.f, 0.f, 0.f };

    float Vector4::operator[](uint8 index) const
    {
        return Scalars[index];
    }

    float& Vector4::operator[](uint8 index)
    {
        return Scalars[index];
    }

    Vector4 Vector4::operator-() const
    {
        return Vector4(-X, -Y, -Z, -W);
    }

    Vector4 Vector4::operator+(const Vector4& v) const
    {
        return Vector4(X + v.X, Y + v.Y, Z + v.Z, W + v.W);
    }

    Vector4 Vector4::operator-(const Vector4& v) const
    {
        return Vector4(X - v.X, Y - v.Y, Z - v.Z, W - v.W);
    }

    Vector4 Vector4::operator*(const float& scalar) const
    {
        return Vector4(X * scalar, Y * scalar, Z * scalar, W * scalar);
    }

    Vector4 Vector4::operator*(const Vector4& v) const
    {
        return Vector4(X * v.X, Y * v.Y, Z * v.Z, W * v.W);
    }

    Vector4 Vector4::operator/(const float& scalar) const
    {
        return Vector4(X / scalar, Y / scalar, Z / scalar, W / scalar);
    }

    Vector4 Vector4::operator*(const Matrix4x4& m) const
    {
        Matrix4x4 transposedM = m.Transpose();
        return Vector4(
            this->Dot(transposedM[0]),
            this->Dot(transposedM[1]),
            this->Dot(transposedM[2]),
            this->Dot(transposedM[3])
        );
    }

    Vector4& Vector4::operator+=(const Vector4& v)
    {
        X += v.X;
        Y += v.Y;
        Z += v.Z;
        W += v.W;
        return *this;
    }

    Vector4& Vector4::operator-=(const Vector4& v)
    {
        X -= v.X;
        Y -= v.Y;
        Z -= v.Z;
        W -= v.W;
        return *this;
    }

    Vector4& Vector4::operator*=(const float& scalar)
    {
        X *= scalar;
        Y *= scalar;
        Z *= scalar;
        W *= scalar;
        return *this;
    }

    Vector4& Vector4::operator*=(const Vector4& v)
    {
        X *= v.X;
        Y *= v.Y;
        Z *= v.Z;
        W *= v.W;
        return *this;
    }

    Vector4& Vector4::operator/=(const float& scalar)
    {
        X /= scalar;
        Y /= scalar;
        Z /= scalar;
        W /= scalar;
        return *this;
    }

    Vector4& Vector4::operator*=(const Matrix4x4& m)
    {
        *this = *this * m;
        return *this;
    }

    bool Vector4::operator==(const Vector4& v)
    {
        return this->Equal(v);
    }

    bool Vector4::Equal(const Vector4& v, float Tolerance)
    {
        return std::abs(X - v.X) <= Tolerance &&
            std::abs(Y - v.Y) <= Tolerance &&
            std::abs(Z - v.Z) <= Tolerance &&
            std::abs(W - v.W) <= Tolerance;
    }

    float Vector4::Size()
    {
        return std::sqrtf(this->SizeSquared());
    }

    float Vector4::SizeSquared() const
    {
        return X * X + Y * Y + Z * Z + W * W;
    }

    Vector4 Vector4::GetNormalize()
    {
        float squareSize = this->SizeSquared();
        if (squareSize == 1.f)
            return *this;

        if (squareSize == 0.f)
            return Vector4::Zero;

        // TODO: fast-sqrt 사용하기
        return *this / std::sqrtf(squareSize);
    }

    void Vector4::Normalize()
    {
        *this = this->GetNormalize();
    }

    float Vector4::Dot(const Vector4& v) const
    {
        return X * v.X + Y * v.Y + Z * v.Z + W * v.W;
    }

    Vector2 Vector4::ToVector2() const
    {
        return Vector2(X, Y);
    }

    Vector3 Vector4::ToVector3() const
    {
        return Vector3(X, Y, Z);
    }

    std::wstring Vector4::ToString() const
    {
        return std::format(L"{:.3f}, {:.3f}, {:.3f}, {:.3f}", X, Y, Z, W);
    }

}