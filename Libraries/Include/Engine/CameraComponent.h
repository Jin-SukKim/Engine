#pragma once
#include "Component.h"

namespace JE {
	class CameraComponent : public Component
	{
		using Super = Component;
	public:

	private:
		ScreenPoint _viewportSize;
		// 카메라에 보이는 범위 (화각) - NDC는 언제나 가운데 (0, 0)을 원점으로
	};
}
