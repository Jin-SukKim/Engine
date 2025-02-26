#pragma once
#include "UIBase.h"

namespace JE {
	class UIButton : public UIBase
	{
		using Super = UIBase;
	public:
		UIButton() : Super(UIType::Button) {}
		virtual ~UIButton() override {}

		virtual void OnInit() override;
		virtual void OnActive() override;
		virtual void OnInActive() override;
		virtual void OnTick(const float& DeltaTime) override;
		virtual void OnRender(IRenderer* r) override;
		virtual void OnClear() override;

		virtual void ButtonClick();
	
	private:
		Texture* _texture;
		Event _onClick;
	};
}
