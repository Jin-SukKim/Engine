#include "pch.h"
#include "UIButton.h"

namespace JE {
	void UIButton::OnInit()
	{
		SetPos(Vector2(200.0f, 200.0f));
		SetSize(Vector2(200.0f, 200.0f));

		_onClick = std::bind(&UIButton::ButtonClick, this);
	}
	void UIButton::OnActive()
	{
	}
	void UIButton::OnInActive()
	{
	}
	void UIButton::OnTick(const float& DeltaTime)
	{
		// TODO: Sample Idea
		//Vector2 mousePos = Input::GetMousePos();

		//if (_pos.x <= mousePos.x && mousePos.x <= _pos.x + _size.x
		//	&& _pos.y <= mousePos.y && mousePos.y <= _pos.y + _size.y)
		//{
		//	_bMouseOn = true;
		//}
		//else
		//{
		//	_bMouseOn = false;
		//}

		//if (Input::GetKeyDown(KeyCode::LButton))
		//{
		//	if (_bMouseOn)
		//	{
		//		mOnClick();
		//	}
		//}
	}
	void UIButton::OnRender(IRenderer* r)
	{
	}
	void UIButton::OnClear()
	{
	}
	void UIButton::ButtonClick()
	{
		// event
	}
}