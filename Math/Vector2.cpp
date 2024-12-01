#include "pch.h"
#include "Vector2.h"
using namespace JE;

const Vector2 Vector2::UnitX = Vector2(1.f, 0.f);
const Vector2 Vector2::UnitY = Vector2(0.f, 1.f);
const Vector2 Vector2::One   = Vector2(1.f, 1.f);
const Vector2 Vector2::Zero  = Vector2(0.f, 0.f);

float Vector2::operator[](uint8 index) const
{
    assert(index < 2); // index가 1보다 큰 값이 들어오면 assert error
    return Scalars[index];
}

float& Vector2::operator[](uint8 index)
{
    assert(index < 2); // index가 1보다 큰 값이 들어오면 assert error
    return Scalars[index];
}

Vector2 Vector2::operator-() const
{
    return Vector2(-X, -Y);
}

Vector2 Vector2::operator+(const Vector2& v) const
{
    return Vector2(X + v.X, Y + v.Y);
}

Vector2 Vector2::operator-(const Vector2& v) const
{
    return Vector2(X - v.X, Y - v.Y);
}

Vector2 Vector2::operator*(const Vector2& v) const
{
    return Vector2(X * v.X, Y * v.Y);
}

Vector2 Vector2::operator*(const float& scalar) const
{
    return Vector2(X * scalar, Y * scalar);
}

Vector2 Vector2::operator/(const float& scalar) const
{
    return Vector2(X / scalar, Y / scalar);
}

Vector2& Vector2::operator+=(const Vector2& v)
{
    X += v.X;
    Y += v.Y;
    return *this;
}

Vector2& Vector2::operator-=(const Vector2& v)
{
    X -= v.X;
    Y -= v.Y;
    return *this;
}

Vector2& Vector2::operator*=(const Vector2& v)
{
    X *= v.X;
    Y *= v.Y;
    return *this;
}

Vector2& Vector2::operator*=(const float& scalar)
{
    X *= scalar;
    Y *= scalar;
    return *this;
}

Vector2& Vector2::operator/=(const float& scalar)
{
    X /= scalar;
    Y /= scalar;
    return *this;
}

bool Vector2::operator==(const Vector2& v)
{
    return Equal(v);
}

bool Vector2::Equal(const Vector2& v, float Tolerance)
{
    // 부동소수점은 실수를 정확히 저장하지 못하므로 약간의 오차범위를 허용
    return std::abs(X - v.X) <= Tolerance && 
        std::abs(Y - v.Y) <= Tolerance;
}

float Vector2::Size() const
{
    return std::sqrtf(SizeSquared());
}

float Vector2::SizeSquared() const
{
    return X * X + Y * Y;
}

Vector2 Vector2::GetNormalize() const
{   
    // 벡터의 크기
    float sizeSq = SizeSquared();
    // 이미 크기가 1이면
    if (sizeSq == 1.f) 
        return *this; // 이미 단위 벡터인 것
    // 크기가 0이면 나눌 수 없고 이미 영벡터
    if (sizeSq == 0.f)
        return Vector2::Zero;

    // TODO: sqrt는 계산이 매우 느리므로 보통 fast-sqrt를 사용하는 것이 좋음
    // 빠르게 sqrt의 근사치를 구하는 방법으로 정확하진 않지만 매우 빠름
    // https://m.blog.naver.com/sksd321/221250598540
    // https://kipl.tistory.com/236
    // https://blog.naver.com/jinhan814/222195179213
    // https://ko.wikipedia.org/wiki/%EA%B3%A0%EC%86%8D_%EC%97%AD_%EC%A0%9C%EA%B3%B1%EA%B7%BC

    // 단위 벡터 = 벡터 / 벡터의 크기
    return Vector2(X, Y) / std::sqrtf(sizeSq);
}

void Vector2::Normalize()
{
    *this = GetNormalize();
}

Vector2 Vector2::Rotate2D(const float& angle) const
{
    // 각도에 해당하는 sin과 cos로 회전된 벡터 계산
    float sin = 0.f, cos = 0.f;
    MathUtils::GetSinCos(sin, cos, angle);
    return Vector2(X * cos - Y * sin, X * sin + Y * cos);
}

float Vector2::Angle() const
{
    // atan2 함수를 사용해 평면의 사분면에 대응하는 각도를 얻기 - (-180, 180) 범위
    return std::atan2f(Y, X);
}

float Vector2::AngleToDegree() const
{
    // (-180, 180) 범위
    return MathUtils::Rad2Deg(Angle());
}

Vector2 Vector2::ToPolarCoordinate() const
{
    // 극좌표계는 원점으로부터의 거리 r과 각도로 구성되며 회전을 관리하고 구현하기 편리
    // ex) 시간에 따른 회전의 움직임 구현, 회전에 관련된 효과를 연출
    return Vector2(Size(), Angle());
}

Vector2 Vector2::ToCartesianCoordinate() const
{
    // 극좌표계 좌표 (거리, 각도), 거리 = 반지름, 각도 = radian => 데카르트 좌표 (x, y)
    return Vector2(X * std::cosf(Y), X * std::sinf(Y));
}

float Vector2::Dot(const Vector2& v) const
{
    return X * v.X + Y * v.Y;
}

Vector2 Vector2::Projection(const Vector2& v) const
{
    // 기준이 되는 벡터는 정규화
    Vector2 unitV = v.GetNormalize();
    // 내적으로 투영된 길이를 구한 뒤 방향벡터를 곱해 투영벡터 생성
    return unitV * this->Dot(unitV);
}

std::wstring Vector2::ToString() const
{
    return std::format(L"{:.3f}, {:.3f}", X, Y);
}
