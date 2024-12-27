#pragma once

namespace JE {

/*
	���Ϸ� ���� ����ϴ� ȸ��
	Yaw = y�� ȸ�� (�¿�), Roll = Z�� ȸ��(ȸ��), Pitch = X�� ȸ��(���Ʒ�)
*/ 
struct Rotator
{
	Rotator() = default;
	Rotator(float yaw, float roll, float pitch) : Yaw(yaw), Roll(roll), Pitch(pitch) {}

	// ���Ϸ� ���� ȸ������ ����ϱ� ���ϰ� �籸��, ���� ���� [0, 360] ������ ����
	void Clamp();

	// ���� ���� [0, 360] ������ ����
	float GetAxisClampValue(float value);

	// ȸ�� ��ȯ�� ���� �� (Roll -> Pitch -> Yaw ȸ�� ���)
	void GetLocalAxes(Vector3& outRight, Vector3& outUp, Vector3& outForward);

	std::wstring ToString() const;

	static const Rotator Identity;

	// ���� �������� ȸ�� (Degree)
	float Yaw = 0.f; // Y�� ȸ��
	float Roll = 0.f; // Z�� ȸ��
	float Pitch = 0.f; // X�� ȸ��
};


}