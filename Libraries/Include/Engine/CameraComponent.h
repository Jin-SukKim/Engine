#pragma once
#include "Component.h"

namespace JE {
	class CameraComponent : public Component
	{
		using Super = Component;
	public:

	private:
		ScreenPoint _viewportSize;
		// ī�޶� ���̴� ���� (ȭ��) - NDC�� ������ ��� (0, 0)�� ��������
	};
}
