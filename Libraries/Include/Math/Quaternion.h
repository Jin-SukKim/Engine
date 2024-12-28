#pragma once

namespace JE {

/*
	q = w + xi + yj + zk = (w, v), 
	w = �Ǽ�, v = (x, y, z) �����

	���������� ������� ����� ȸ������ �����ο� ȸ�� ���� (Rotator -> Quaternion -> ȸ�� ��� -> ������)
	�Ǽ��ΰ� 0�� ������� ����� �����(Pure Imaginary Quaternion)�̶� �θ��� �� ����� �����Ǳ⿡ 3���� ���� ������ ����
*/ 
struct Quaternion
{
	Quaternion() = default;
	// v = (X, Y, Z) �����, W = �Ǽ�
	Quaternion(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) {}
	// ȸ���� n�� ȸ���� ������ ȸ�� ����� ����
	Quaternion(const Vector3& axis, float angleDegree);
	// ���Ϸ� ��(Rotator)���κ��� ����� ����
	Quaternion(const Rotator& rotator);
	// ȸ�� ��ȯ��ķκ��� ����� ����
	Quaternion(const Matrix3x3& mat);

	Quaternion operator-() const;
	Quaternion operator+(const Quaternion& q) const;
	Quaternion operator-(const Quaternion& q) const;
	Quaternion operator+=(const Quaternion& q);
	Quaternion operator-=(const Quaternion& q);
	// ������� ���� (�ع��� ��)
	Quaternion operator*(const Quaternion& q) const;
	Quaternion operator*=(const Quaternion& q);

	// ������� ũ�� (Norm)
	float NormSize() const;
	// ����ȭ
	void Normalize();
	// ũ�Ⱑ 1�� ���� �����
	bool IsUnitQuaternion() const;

	// �ӷ� ����� : q* = w - (xi + yj + zk) = (w, -v), ���� ������� ��� �ݴ� ���� ȸ��
	Quaternion Conjugate() const;

	// ���� ������� ��� ������ �ӷ� ������� ���� (�� ���������� ���� ������� ������ ã�´ٰ� ����)
	Quaternion Inverse() const;

	// ȸ�� ����� q(����� �����)�� ����� ������ ȸ������ �߽����� ������ŭ ȸ�� (��-�� ȸ���� ����)
	Vector3 RotateVector(const Vector3& v) const;
	// ȸ�� ������� ���͸� ȸ��
	Vector3 operator*(const Vector3& v) const;

	// ���Ϸ� ������ ������� ��ȯ�� ����� ����
	void FromRotator(const Rotator& rotator);

	// ��������� ���Ϸ� ������ ��ȯ
	Rotator ToRotator() const;

	// ������ X�� ȸ�� ������� ȸ��
	static Vector3 GetLocalX(const Quaternion& q) { return q * Vector3::UnitX; }
	// ������ Y�� ȸ�� ������� ȸ��
	static Vector3 GetLocalY(const Quaternion& q) { return q * Vector3::UnitY; }
	// ������ Z�� ȸ�� ������� ȸ��
	static Vector3 GetLocalZ(const Quaternion& q) { return q * Vector3::UnitZ; }

	// ��������� ȸ�� ��ȯ��ķ� ��ȯ (������� ȸ���� �� ������ ���͸� ����� ȸ�� ��ȯ��� ����)
	static Matrix4x4 GetRotationTransform(const Quaternion& q);

	// ȸ�� ��ȯ��Ŀ��� ������� ��ȯ
	void FromMatrix(const Matrix3x3& mat);

	// ������� ���� ���� ���� (������ �࿡ ���� ȸ�� ����) - ������ �̿��� ������ �ε巯�� ȸ��
	static Quaternion Slerp(const Quaternion& quat1, const Quaternion& quat2, float ratio);

	// ������� �Ǽ��� W
	float RealPart() const { return W; }
	// ������� �����, 3���� ���� v
	Vector3 ImaginaryPart() const { return Vector3(X, Y, Z); }

	// ������� �����ϱ� ���� ���Ϸ� ������ ��ȯ�� �� ���
	std::wstring ToString() const;

	static const Quaternion Identity;

	// �����
	float X = 0.f;
	float Y = 0.f;
	float Z = 0.f;
	// �Ǽ���
	float W = 1.f;
};

}
