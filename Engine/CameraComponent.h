#pragma once
#include "Component.h"

namespace JE {
	class TransformComponent;

	class CameraComponent : public Component
	{
		using Super = Component;
	public:

	private:
		ScreenPoint _viewportSize;
		// 카메라에 보이는 범위 (화각) - NDC는 언제나 가운데 (0, 0)을 원점으로
		float _fov = 60.f;
		float _nearZ = 5.5f;
		float _farZ = 5000.f;

		Frustum _veiwFrustum;

		std::unique_ptr<TransformComponent> _cameraTransform;
	};
}

//Matrix4x4 CameraActor::GetOrthographicMatrix() const
//{
//    const float halfHeight = _viewportSize.GetHeight() * 0.5f;
//    const float halfWidth = _viewportSize.GetWidth() * 0.5f;
//    const float invNF = 1.0f / (_farZ - _nearZ);  // scale factor
//
//    return Matrix4x4(
//        Vector4(1.0f / halfWidth, 0.0f, 0.0f, 0.0f),
//        Vector4(0.0f, 1.0f / halfHeight, 0.0f, 0.0f),
//        Vector4(0.0f, 0.0f, invNF, 0.0f),         // scale: 1/(far-near)
//        Vector4(0.0f, 0.0f, -_nearZ * invNF, 1.0f) // offset: -near/(far-near)
//    );
//}