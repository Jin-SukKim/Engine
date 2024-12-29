#pragma once

namespace JE {
	class InputManager
	{
	public:
		void UpdateInput();

		// Input와 함수를 연결
		template<typename T>
		void BindInputAxis(InputAxis axis, InputAxisAction action, T* object, void(T::* func)(float));
		template<typename T>
		void BindInputAction(InputAction action, InputState state, T* object, void(T::* func)());

		// InputAxis의 값
		float GetAxis(InputAxis inputAxis) const;
		// InputButton이 눌렸는지
		bool GetButton(InputButton button) const;
	private:
		// 등록된 InputAxisAction 맵
		std::array<InputAxisAction, static_cast<size_t>(InputAxis::MaxAxis)> _axisMap;
		// 등록된 InputAction 맵
		std::array<InputAction, static_cast<size_t>(InputButton::MaxButtons)> _buttonMap;
	};


	template<typename T>
	inline void InputManager::BindInputAxis(InputAxis axis, InputAxisAction action, T* object, void(T::* func)(float))
	{
		action.BindInputAxis(object, func);
		_axisMap[static_cast<size_t>(axis)] = action;
	}

	template<typename T>
	inline void InputManager::BindInputAction(InputAction action, InputState state, T* object, void(T::* func)())
	{
		action.BindAction(object, func, state);
		_buttonMap[static_cast<size_t>(action.GetButton())] = action;
	}
};
