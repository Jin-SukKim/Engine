#pragma once
#include "UIBase.h"

namespace JE {
	class UIHUD : public UIBase
	{
		using Super = UIBase;
	public:
		UIHUD() : Super(UIType::HUD) {}
		virtual ~UIHUD() override {}

		virtual void OnInit() override;
		virtual void OnActive() override;
		virtual void OnInActive() override;
		virtual void OnTick(const float& DeltaTime) override;
		virtual void OnRender(IRenderer* r) override;
		virtual void OnClear() override;

	private:
		Texture* _texture;
	};
}
