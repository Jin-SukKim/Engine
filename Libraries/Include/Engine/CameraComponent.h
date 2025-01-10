#pragma once
#include "Component.h"
#include "TransformComponent.h"

namespace JE {
	class Actor;

	class CameraComponent : public Component
	{
		using Super = Component;
	public:
		CameraComponent(const std::wstring& name);
		virtual ~CameraComponent() override {}

		virtual void Tick(const float& DeltaTime) override;

		// ����ü ����
		void CreateFrustum();
		// �𵨸� ����� �޾� �𵨸� ���(����) * �� ��� * ���� ���� ����� �����
		// ���� ������ ��ǥ�� �޾� ����ü �ø��� ������ �� �ִ� ����ü�� ����
		void CreateFrustum(const Matrix4x4& mat);
		// �� ���������� ����ü �ø��� ���� ����ü
		const Frustum& GetFrustum() const { return _viewFrustum; }

		// ī�޶��� Viewport ũ��
		const ScreenPoint& GetVeiwportSize() const { return _viewportSize; }
		void SetViewportSize(const ScreenPoint& size) { _viewportSize = size; }

		/*
			ī�޶� ���� ���⿡ ���� ������ ����� ȸ���� ���� ���� ������ ȸ�� ��Ŀ� ���
			���� ������ Up������ y���� ��������� ī�޶� �������� ��찡 ���� �� ������ up ���͸� Parameter�� ����
			ī�޶� �������� ��� up ���͸� -Vector3::UnitY�� �޾ƾ� ��

			������ TargetPos�� ������ �ε��� ī�޶� ȸ��
		*/
		void SetLookAt(const Vector3& targetPos, const Vector3& up = Vector3::UnitY);
		void SetLookAt(Object* object, const Vector3& up = Vector3::UnitY);

		// ī�޶� ȭ�� ����
		void SetFOV(float fov) { _fov = fov; }
		const float GetFOV() const { return _fov; }

		const Vector3 GetViewDir() const;

		// �� ��� (�̵� -> ȸ���� ����ȯ�� ����)
		Matrix4x4 GetViewMatrix() const;
		// �� ��Ŀ� ȸ���� �����ؼ� ��������
		Matrix4x4 GetViewRotationMatrix() const;
		/*
			���� ���� ��� (�� ������ ���� ���� ���(NDC) ���� ������ ������Ű�� ���� Ŭ�� ��ǥ��� ��ȯ�ϴ� ���)
			�� ��ǥ(�� ���) -> Ŭ�� ��ǥ(���� ���� ���) -> NDC ��ǥ(�� ������ ����° ���� Z������ ������)
			Ŭ�� ��ǥ�� W ���� �ᱹ �� ������ ���� ���� �ǹ��ϰ� �̸� �����ָ� Z ���� NDC ��ǥ�� ���� ���� ��
		*/
		Matrix4x4 GetPerspectiveMatrix() const;
		// �� x ���� ���
		Matrix4x4 GetViewPerspectiveMatrix() const;

		// ���� ���� ���
		Matrix4x4 GetOrthographicMatrix() const;

		// ī�޶��� Transform
		TransformComponent* GetTransform() const { return _cameraTransform.get(); }
		void GetViewAxes(Vector3& outViewX, Vector3& outViewY, Vector3& outViewZ) const;

		void SetNearZ(const float& nearZ) { _nearZ = nearZ;	}
		float GetNearZ() const { return _nearZ; }

		void SetFarZ(const float& farZ) { _farZ = farZ;	}
		float GetFarZ() const { return _farZ; }

		// Actor�� ī�޶� ����
		void AttachCamera(Actor* actor) { _attached = actor; }
		void DettachCamera() { _attached = nullptr; }

		void SetMinDistance(float distance) { _minDistance = distance; }
		void SetLerpSpeed(float speed) { _lerpSpeed = speed; }
	private:
		ScreenPoint _viewportSize;
		/*
			ī�޶� ���̴� ���� (ȭ��) - NDC�� ������ ��� (0, 0)�� �������� �¿���� ���� 1�� ���� ����
			ī�޶��� ȭ���� ���� �� ���� ��鿡 �����ϴ� NDC�� ������ ���� �������ϹǷ� �����Ÿ��� ����
			tan(fov/2) = 1 / d, d = �����Ÿ�, d = 1 / (tan(fov / 2))
		*/ 
		float _fov = 60.f;
		float _nearZ = 5.5f;
		float _farZ = 5000.f;

		Frustum _viewFrustum;

		std::unique_ptr<TransformComponent> _cameraTransform;

		// owner�� �� ���� �ʰ� ���󰡴� �ð��� ȿ���� Lagging ȿ���� ���� �Ÿ�
		float _minDistance = 0.f;
		float _lerpSpeed = 1.f; // ī�޶� ���󰡴� �ӵ�

		Actor* _attached = nullptr;
	};
}
