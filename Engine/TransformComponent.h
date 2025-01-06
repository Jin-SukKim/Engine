#pragma once
#include "Component.h"

namespace JE {
	class TransformComponent : public Component
	{
		using Super = Component;
	public:
		TransformComponent(const std::wstring& name) : Super(name, ComponentType::Transform) {}
		virtual ~TransformComponent() override {}

		// Position
		void SetPos(const Vector3& pos) { _pos = pos; }
		void AddPos(const Vector3& deltaPos) { _pos += deltaPos; }
		Vector3 GetPos() const { return _pos; }

		// Rotation (ȸ���ϸ� �� axis�� �൵ ȸ����) - ���� �ý����� Quaternion�� ����� ȸ��
		// ���Ϸ� ���� ����ϴ� ȸ�� (Yaw = y�� ȸ�� (�¿�), Roll = Z�� ȸ��(ȸ��), Pitch = X�� ȸ��(���Ʒ�))
		void SetRotation(const Rotator& rotator) { _rotation = Quaternion(rotator); }
		// ȸ�� ��ȯ���
		void SetRotation(const Matrix3x3& mat) { _rotation = Quaternion(mat); }
		// ������� �̿��� ȸ��
		void SetRotation(const Quaternion& q) { _rotation = q; }

		// �߰� ȸ��
		void AddYaw(const float& deltaDegree);
		void AddRoll(const float& deltaDegree);
		void AddPitch(const float& deltaDegree);
		
		Quaternion GetRotation() const { return _rotation; }

		// Scale
		void SetScale(const Vector3& scale) { _scale = scale;	}
		void AddScale(const Vector3& deltaScale) { _scale += deltaScale; }
		Vector3 GetScale() const { return _scale; }

		// Local Axes
		// ȸ�� ������� ������ ���͸� ȸ��
		const Vector3 GetLocalX() const { return _rotation * _right; }
		const Vector3 GetLocalY() const { return _rotation * _up; }
		const Vector3 GetLocalZ() const { return _rotation * _forward; }

		// ���� �⺻���� ũ�� -> ȸ�� -> �̵���ȯ
		Matrix4x4 GetTransformMatrix() const;
		// �̵� ��ȯ ���
		Matrix4x4 GetTranslationMatrix() const;
		// ȸ�� ��ȯ ���
		Matrix4x4 GetRotationMatrix() const;
		// ũ�� ��ȯ ���
		Matrix4x4 GetScaleMatrix() const;

		// �̵� ����ȯ ���
		Matrix4x4 GetInvTranslationMatrix() const;
		// ȸ�� ����ȯ ���
		Matrix4x4 GetInvRotationMatrix() const;
		// ũ�� ����ȯ ���
		Matrix4x4 GetInvScaleMatrix() const;

		// ���� �� ����
		void SetLocalAxes(const Vector3& right, const Vector3& up, const Vector3& forward) {
			_right = right;
			_up = up;
			_forward = forward;
		}
	private:
		// ��ü�� �������� ������ ���� ������ Local Axis�� ȸ������ ���� ȸ���ϹǷ� ȸ���� ������ ����
		// ���Ϸ� ���� ����� ȸ���Ҷ� ��� (������� ����ϸ� ������� ����)
		void updateAxes();

	private:
		Vector3 _pos = Vector3::Zero;
		Quaternion _rotation;
		Vector3 _scale = Vector3::One;

		// Local ��
		Vector3 _right = Vector3::UnitX;
		Vector3 _up = Vector3::UnitY;
		Vector3 _forward = Vector3::UnitZ;
	};
}

