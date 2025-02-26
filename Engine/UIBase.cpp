#include "pch.h"
#include "UIBase.h"

namespace JE {
	void UIBase::Init()
	{
		OnInit();
	}
	void UIBase::Tick(const float& DeltaTime)
	{
		if (_bEnabled)
			OnTick(DeltaTime);
	}
	void UIBase::Render(IRenderer* r)
	{
		if (_bEnabled)
			OnRender(r);
	}
	void UIBase::Active()
	{
		_bEnabled = true;
		OnActive();
	}
	void UIBase::InActive()
	{
		_bEnabled = false;
		OnInActive();
	}
	void UIBase::UIClear()
	{
		OnClear();
	}
}