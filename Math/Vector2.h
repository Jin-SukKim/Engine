#pragma once

namespace JE {

struct Vector2
{
	Vector2() : x(0.f), y(0.f) {}
	Vector2(float scalar) : x(scalar), y(scalar) {}
	Vector2(float x, float y) : x(x), y(y) {}
	Vector2(int x, int y) : x(static_cast<float>(x)), y(static_cast<float>(y)) {}
	Vector2(const Vector2& vect) : x(vect.x), y(vect.y) {}

	float operator[](uint8 index) const;
	float& operator[](uint8 index);
	Vector2 operator-() const; // ������ ��ȣ ����

	// ������ dhqjfheld
	Vector2 operator+(const Vector2& v) const;
	Vector2 operator-(const Vector2& v) const;
	Vector2 operator*(const Vector2& v) const;
	Vector2 operator*(const float& scalar) const;
	Vector2 operator/(const float& scalar) const;
	Vector2& operator+=(const Vector2& v);
	Vector2& operator-=(const Vector2& v);
	Vector2& operator*=(const Vector2& v);
	Vector2& operator*=(const float& scalar);
	Vector2& operator/=(const float& scalar);

	bool operator==(const Vector2& v);
	// ��� ���� ���� = SMALL_NUMBER
	bool Equal(const Vector2& v, float Tolerance = SMALL_NUMBER);

	float Size() const;
	float SizeSquared() const; // sqrt ������ ����� ũ�Ƿ� �������� ����ϴ� ��찡 ����

	// ����ȭ : ũ�Ⱑ 1�� ���� ���� (Unit Vector)
	Vector2 GetNormalize() const;
	// ���� ���͸� ����ȭ
	void Normalize();

	// �ﰢ�Լ��� Ȱ���� 2D ȸ�� (angle = degree)
	Vector2 Rotate2D(const float& angle) const;

	// ���Ͱ� x��� �̷�� (-180, 180) ������ ���̰� 
	float Angle() const; // Radian
	float AngleToDegree() const; // Degree

	// ȸ���� ���� ����ǥ��� ��ȯ (����ǥ���� ���� ���� [-PI, PI]) : ����ǥ�� = (Size, Angle)
	Vector2 ToPolarCoordinate() const;
	// ����ǥ�迡�� ��ī��Ʈ ��ǥ��� ��ȯ
	Vector2 ToCartesianCoordinate() const;
	 
	/*
		���� = |u||v|cos���� : �� ������ ���̰��� ���� cos�Լ��� ���, �� ���Ͱ� ���� ���͸� ���̰�
		������ ���� 0�̸� cos 90, 270(-90)���� �� ���Ͱ� �����ϴ� ���
		������ ���� (-90, 90) ������ ���, -90�� 90���� 0(����), ������ �������� ����
	*/  
	float Dot(const Vector2& v) const;
	
	// ���� v�� ����
	Vector2 Projection(const Vector2& v) const;

	// print
	std::wstring ToString() const;

	// �̸� ������ �� ���� ����
	static const Vector2 UnitX;
	static const Vector2 UnitY;
	static const Vector2 One;
	static const Vector2 Zero;

	union {
		struct {
			float x, y;
		};
		std::array<float, static_cast<uint8>(2)> Scalars = { 0.f, 0.f };
	};
};

}