#include "pch.h"
#include "Camera2DComponent.h"
#include "SpriteActor.h"
#include "Transform2DComponent.h"

namespace JE {
	Camera2DComponent::Camera2DComponent(const std::wstring& name) : Super(name, ComponentType::Camera) {
		_cameraTransform = std::make_unique<Transform2DComponent>(L"CameraTransform2D");
	}

	void Camera2DComponent::Tick(const float& DeltaTime)
	{
		if (_attached) {
			Transform2DComponent* tr = GetOwner()->GetComponent<Transform2DComponent>();
			if (tr) {
				Vector2 ownerPos = tr->GetPos();
				Vector2 cameraPos = _cameraTransform->GetPos();
				
				// 플레이어를 한 템포 늦게 따라가는 시각적 효과인 Lagging 효과
				// minDistance == 0이면 Lagging 효과없이 바로 따라가기
				if (_minDistance == 0.f || (ownerPos - cameraPos).SizeSquared() < _minDistance * _minDistance)
					_cameraTransform->SetPos(ownerPos);
				else {
					// 현재 프레임에서 쫓아갈 비율 계산
					float ratio = std::clamp(_lerpSpeed * DeltaTime, 0.f, 1.f);
					// 카메라의 현재 위치에서 플레이어의 현재 위치까지 이동하는 벡터를 구하고 ratio를 곱해 이동할 위치 계산
					Vector2 newPos = MathUtils::Lerp(cameraPos, ownerPos, ratio);
					_cameraTransform->SetPos(newPos);
				}
			}
		}
	}

	Matrix3x3 Camera2DComponent::GetViewMatrix() const
	{
		// 카메라의 이동은 마치 캐릭터의 움직임의 반대와 같음
		return Matrix3x3(
			Vector3::UnitX,
			Vector3::UnitY,
			Vector3(-_cameraTransform->GetPos(), 1.f)
		);
	}

}