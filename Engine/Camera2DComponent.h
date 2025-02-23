#pragma once
#include "Component.h"
#include "Transform2DComponent.h"

namespace JE {
	class SpriteActor;

	class Camera2DComponent : public Component
	{
		using Super = Component;
	public:
		Camera2DComponent(const std::wstring& name);
		virtual ~Camera2DComponent() override {}

		virtual void Tick(const float& DeltaTime) override;

		// ī�޶� viewport ũ��
		void SetViewportSize(const ScreenPoint& size) { _viewportSize = size; }
		const ScreenPoint& GetViewportSize() const { return _viewportSize; }
		
		// �� ���
		Matrix3x3 GetViewMatrix() const;

		// Actor�� ī�޶� ����
		void AttachCamera(SpriteActor* actor) { _attached = actor; }
		void DettachCamera() { _attached = nullptr; }

		void SetMinDistance(float distance) { _minDistance = distance; }
		void SetLerpSpeed(float speed) { _lerpSpeed = speed; }

		// ī�޶��� Transform
		Transform2DComponent* GetTransform() const { return _cameraTransform.get(); }
	private:
		// ī�޶� viewport ũ��
		ScreenPoint _viewportSize;
		// ī�޶� ��ü�� transform
		std::unique_ptr<Transform2DComponent> _cameraTransform;
		// owner�� �� ���� �ʰ� ���󰡴� �ð��� ȿ���� Lagging ȿ���� ���� �Ÿ�
		float _minDistance = 0.f; 
		float _lerpSpeed = 1.f; // ī�޶� ���󰡴� �ӵ�

		// ī�޶� Component�� ���� Actor
		SpriteActor* _attached = nullptr;
	};
}

