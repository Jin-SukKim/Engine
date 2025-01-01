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

		virtual void Tick(const float& DeltaTime) override;

		// 카메라 viewport 크기
		void SetViewportSize(const ScreenPoint& size) { _viewportSize = size; }
		const ScreenPoint& GetVeiwportSize() const { return _viewportSize; }
		
		// 뷰 행렬
		Matrix3x3 GetViewMatrix() const;

		// Actor에 카메라 부착
		void AttachCamera() { _attached = true; }
		void DettachCamera() { _attached = false; }

		void SetMinDistance(float distance) { _minDistance = distance; }
		void SetLerpSpeed(float speed) { _lerpSpeed = speed; }
	private:
		// 카메라 viewport 크기
		ScreenPoint _viewportSize;
		// 카메라 자체의 transform
		std::unique_ptr<Transform2DComponent> _cameraTransform;
		bool _attached = false;
		// owner를 한 템포 늦게 따라가는 시각적 효과인 Lagging 효과를 위한 거리
		float _minDistance = 0.f; 
		float _lerpSpeed = 1.f; // 카메라가 따라가는 속도
	};
}

