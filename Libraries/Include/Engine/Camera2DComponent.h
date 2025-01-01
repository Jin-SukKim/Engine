#pragma once
#include "Component.h"

namespace JE {
	class SpriteActor;
	class Transform2DComponent;

	class Camera2DComponent : public Component
	{
		using Super = Component;
	public:
		Camera2DComponent(const std::wstring& name);
		virtual ~Camera2DComponent() override {}

		// ī�޶� viewport ũ��
		void SetViewportSize(const ScreenPoint& size) { _viewportSize = size; }
		const ScreenPoint& GetVeiwportSize() const { return _viewportSize; }
		
		// �� ���
		Matrix3x3 GetViewMatrix() const;

		// Actor�� ī�޶� ����
		void AttachCamera(SpriteActor* owner);
	private:
		ScreenPoint _viewportSize;
		std::unique_ptr<Transform2DComponent> _cameraTransform;
		Transform2DComponent* _attached = nullptr;
	};
}

