#include "pch.h"
#include "Vector2.h"
using namespace JE;

const Vector2 Vector2::UnitX = Vector2(1.f, 0.f);
const Vector2 Vector2::UnitY = Vector2(0.f, 1.f);
const Vector2 Vector2::One   = Vector2(1.f, 1.f);
const Vector2 Vector2::Zero  = Vector2(0.f, 0.f);

float Vector2::operator[](uint8 index) const
{
    assert(index < 2); // index�� 1���� ū ���� ������ assert error
    return Scalars[index];
}

float& Vector2::operator[](uint8 index)
{
    assert(index < 2); // index�� 1���� ū ���� ������ assert error
    return Scalars[index];
}

Vector2 Vector2::operator-() const
{
    return Vector2(-x, -y);
}

Vector2 Vector2::operator+(const Vector2& v) const
{
    return Vector2(x + v.x, y + v.y);
}

Vector2 Vector2::operator-(const Vector2& v) const
{
    return Vector2(x - v.x, y - v.y);
}

Vector2 Vector2::operator*(const Vector2& v) const
{
    return Vector2(x * v.x, y * v.y);
}

Vector2 Vector2::operator*(const float& scalar) const
{
    return Vector2(x * scalar, y * scalar);
}

Vector2 Vector2::operator/(const float& scalar) const
{
    return Vector2(x / scalar, y / scalar);
}

Vector2& Vector2::operator+=(const Vector2& v)
{
    x += v.x;
    y += v.y;
    return *this;
}

Vector2& Vector2::operator-=(const Vector2& v)
{
    x -= v.x;
    y -= v.y;
    return *this;
}

Vector2& Vector2::operator*=(const Vector2& v)
{
    x *= v.x;
    y *= v.y;
    return *this;
}

Vector2& Vector2::operator*=(const float& scalar)
{
    x *= scalar;
    y *= scalar;
    return *this;
}

Vector2& Vector2::operator/=(const float& scalar)
{
    x /= scalar;
    y /= scalar;
    return *this;
}

bool Vector2::operator==(const Vector2& v)
{
    return Equal(v);
}

bool Vector2::Equal(const Vector2& v, float Tolerance)
{
    // �ε��Ҽ����� �Ǽ��� ��Ȯ�� �������� ���ϹǷ� �ణ�� ���������� ���
    return std::abs(x - v.x) <= Tolerance && 
        std::abs(y - v.y) <= Tolerance;
}

float Vector2::Size() const
{
    return std::sqrtf(SizeSquared());
}

float Vector2::SizeSquared() const
{
    return x * x + y * y;
}

Vector2 Vector2::GetNormalize() const
{   
    // ������ ũ��
    float sizeSq = SizeSquared();
    // �̹� ũ�Ⱑ 1�̸�
    if (sizeSq == 1.f) 
        return *this; // �̹� ���� ������ ��
    // ũ�Ⱑ 0�̸� ���� �� ���� �̹� ������
    if (sizeSq == 0.f)
        return Vector2::Zero;

    // TODO: sqrt�� ����� �ſ� �����Ƿ� ���� fast-sqrt�� ����ϴ� ���� ����
    // ������ sqrt�� �ٻ�ġ�� ���ϴ� ������� ��Ȯ���� ������ �ſ� ����
    // https://m.blog.naver.com/sksd321/221250598540
    // https://kipl.tistory.com/236
    // https://blog.naver.com/jinhan814/222195179213
    // https://ko.wikipedia.org/wiki/%EA%B3%A0%EC%86%8D_%EC%97%AD_%EC%A0%9C%EA%B3%B1%EA%B7%BC

    // ���� ���� = ���� / ������ ũ��
    return Vector2(x, y) / std::sqrtf(sizeSq);
}

void Vector2::Normalize()
{
    *this = GetNormalize();
}

Vector2 Vector2::Rotate2D(const float& angle) const
{
    // ������ �ش��ϴ� sin�� cos�� ȸ���� ���� ���
    float sin = 0.f, cos = 0.f;
    MathUtils::GetSinCos(sin, cos, angle);
    return Vector2(x * cos - y * sin, x * sin + y * cos);
}

float Vector2::Angle() const
{
    // atan2 �Լ��� ����� ����� ��и鿡 �����ϴ� ������ ��� - (-180, 180) ����
    return std::atan2f(y, x);
}

float Vector2::AngleToDegree() const
{
    // (-180, 180) ����
    return MathUtils::Rad2Deg(Angle());
}

Vector2 Vector2::ToPolarCoordinate() const
{
    // ����ǥ��� �������κ����� �Ÿ� r�� ������ �����Ǹ� ȸ���� �����ϰ� �����ϱ� ��
    // ex) �ð��� ���� ȸ���� ������ ����, ȸ���� ���õ� ȿ���� ����
    return Vector2(Size(), Angle());
}

Vector2 Vector2::ToCartesianCoordinate() const
{
    // ����ǥ�� ��ǥ (�Ÿ�, ����), �Ÿ� = ������, ���� = radian => ��ī��Ʈ ��ǥ (x, y)
    return Vector2(x * std::cosf(y), x * std::sinf(y));
}

float Vector2::Dot(const Vector2& v) const
{
    return x * v.x + y * v.y;
}

Vector2 Vector2::Projection(const Vector2& v) const
{
    // ������ �Ǵ� ���ʹ� ����ȭ
    Vector2 unitV = v.GetNormalize();
    // �������� ������ ���̸� ���� �� ���⺤�͸� ���� �������� ����
    return unitV * this->Dot(unitV);
}

std::wstring Vector2::ToString() const
{
    return std::format(L"{:.3f}, {:.3f}", x, y);
}
