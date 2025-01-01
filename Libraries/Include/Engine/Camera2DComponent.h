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

		// 카메라 viewport 크기
		void SetViewportSize(const ScreenPoint& size) { _viewportSize = size; }
		const ScreenPoint& GetVeiwportSize() const { return _viewportSize; }
		
		// 뷰 행렬
		Matrix3x3 GetViewMatrix() const;

		// Actor에 카메라 부착
		void AttachCamera(SpriteActor* owner);
	private:
		ScreenPoint _viewportSize;
		std::unique_ptr<Transform2DComponent> _cameraTransform;
		Transform2DComponent* _attached = nullptr;
	};
}

