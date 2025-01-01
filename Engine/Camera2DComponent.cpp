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
				
				// �÷��̾ �� ���� �ʰ� ���󰡴� �ð��� ȿ���� Lagging ȿ��
				// minDistance == 0�̸� Lagging ȿ������ �ٷ� ���󰡱�
				if (_minDistance == 0.f || (ownerPos - cameraPos).SizeSquared() < _minDistance * _minDistance)
					_cameraTransform->SetPos(ownerPos);
				else {
					// ���� �����ӿ��� �Ѿư� ���� ���
					float ratio = std::clamp(_lerpSpeed * DeltaTime, 0.f, 1.f);
					// ī�޶��� ���� ��ġ���� �÷��̾��� ���� ��ġ���� �̵��ϴ� ���͸� ���ϰ� ratio�� ���� �̵��� ��ġ ���
					Vector2 newPos = MathUtils::Lerp(cameraPos, ownerPos, ratio);
					_cameraTransform->SetPos(newPos);
				}
			}
		}
	}

	Matrix3x3 Camera2DComponent::GetViewMatrix() const
	{
		// ī�޶��� �̵��� ��ġ ĳ������ �������� �ݴ�� ����
		return Matrix3x3(
			Vector3::UnitX,
			Vector3::UnitY,
			Vector3(-_cameraTransform->GetPos(), 1.f)
		);
	}

}