#include "pch.h"
#include "TransformComponent.h"

namespace JE {
	void TransformComponent::AddYaw(const float& deltaDegree)
	{
		Rotator r = _rotation.ToRotator(); // ����� -> ���Ϸ� ������ ��ȯ
		r.Yaw += deltaDegree; // ȸ���� ���ϱ�
		r.Clamp(); // ���Ϸ� ȸ������ ����ϱ� ���ϰ� �籸�� [0, 360] ����
		_rotation = Quaternion(r); // ���Ϸ� �� -> ������� ��ȯ
	}
	void TransformComponent::AddRoll(const float& deltaDegree)
	{
		Rotator r = _rotation.ToRotator();
		r.Roll += deltaDegree;
		r.Clamp();
		_rotation = Quaternion(r);
	}
	void TransformComponent::AddPitch(const float& deltaDegree)
	{
		Rotator r = _rotation.ToRotator(); 
		r.Pitch += deltaDegree; 
		r.Clamp();
		_rotation = Quaternion(r);
	}
	Matrix4x4 TransformComponent::GetTransformMatrix() const
	{
		/*
			�߰����� ������ ����� �ʿ� (�� ����� �����ϴ� ���� + �� ��ĳ��� �Ź� ���)
			return GetScaleMatrix() * GetRotationMatrix() * GetTranslation Matrix();

			���� ��� (���� return���� ��� ����� ����)

			�� ����� ��õ�ϴµ� TransformComponent�� ������ �ִ� ���鸸 ������ 
			����� �����ϱ⿡ �ּ����� ������ ������� ����� ����

			Vector�� ���� + ������ �ִ� �����θ� ���
		*/
		return Matrix4x4(
			// GetLocal() �Լ��� ȸ�� ������� ���� ȸ���� �������� �������� scale�� pos�� ����
			Vector4(GetLocalX() * _scale.X, 0.f),
			Vector4(GetLocalY() * _scale.Y, 0.f),
			Vector4(GetLocalZ() * _scale.Z, 0.f),
			Vector4(_pos, 1.f)
		);
	}
	Matrix4x4 TransformComponent::GetTranslationMatrix() const
	{
		return Matrix4x4::TranslationTransform(_pos);
	}
	Matrix4x4 TransformComponent::GetRotationMatrix() const
	{
		// ������� ����� ����� ���� ���� Ȱ���� ȸ�� ��� ���
		return Matrix4x4::RotationTransform(GetLocalX(), GetLocalY(), GetLocalZ());
	}
	Matrix4x4 TransformComponent::GetScaleMatrix() const
	{
		return Matrix4x4::ScaleTransform(_scale);
	}
	Matrix4x4 TransformComponent::GetInvTranslationMatrix() const
	{
		return Matrix4x4::InverseTranslationTransform(GetTranslationMatrix());
	}
	Matrix4x4 TransformComponent::GetInvRotationMatrix() const
	{
		return Matrix4x4::InversRotationTransform(GetRotationMatrix());
	}
	Matrix4x4 TransformComponent::GetInvScaleMatrix() const
	{
		return Matrix4x4::InverseScaleTransform(GetScaleMatrix());
	}
	void TransformComponent::updateAxes()
	{
		// ���Ϸ� ������ ����� ȸ������ �� ������ ���ϰ� ȸ������ ����
		//_rotation.Clamp();
		//_rotation.SetLocalAxes(_right, _up, _forward);
	}
}