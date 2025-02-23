#pragma once

namespace JE {
	/*
		TODO: ���� frame�� ���ŵǴ� UpdateInput�� ����Ǵ� ���� Key�� �ԷµǾ�� �ν��� ���ٵ�,
		�Է�Ű�� ���� queue�� �÷��ΰ� UpdateInput()�� ����� �� �ݿ��ϸ� Ű ���� ������ �پ�� �� ����.
		�׳� ������ �� ������ �õ��غ��� �ʾ���

		�Է��� ���� queue�� �÷��δ� ���� ����غ��� (���� ��� �޾ƾ� ���� queue�� ����� �� ������?)
	*/
	class InputManager
	{
	public:
		void UpdateInput();

		// Input�� �Լ��� ����
		template<typename T>
		void BindInputAxis(InputAxis axis, InputAxisAction action, T* object, void(T::* func)(float));
		template<typename T>
		void BindInputAction(InputAction action, InputState state, T* object, void(T::* func)());

		// InputAxis�� ��
		float GetAxis(InputAxis inputAxis) const;
		// InputButton�� ���ȴ���
		bool GetButton(InputButton button) const;
	private:
		// ��ϵ� InputAxisAction ��
		std::array<InputAxisAction, static_cast<size_t>(InputAxis::MaxAxis)> _axisMap;
		// ��ϵ� InputAction ��
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
