#pragma once
#include "InputTypes.h"
#include <functional>

namespace JE {
	struct InputAxisAction {
		InputAxisAction() = default;
		InputAxisAction(InputButton upButton, InputButton downButton) 
			: UpButton(upButton), DownButton(downButton) {}

		template<typename T>
		void BindInputAxis(T* object, void(T::* func)(float)) {
			BindFunc = [object, func](float value) {
				(object->*func)(value);
				};
		}

		float GetAxisInput() {
			bool isUp = ::GetAsyncKeyState(static_cast<uint32>(UpButton));
			bool isDown = ::GetAsyncKeyState(static_cast<uint32>(DownButton));

			// XOR�� �� �� �ϳ��� true�� pass
			if (isUp ^ isDown)
				return isDown ? -1.f : 1.f;
				
			// �� �� false�̰ų� true�� ���
			return 0.f;
		}

		void Execute() {
			if (BindFunc)
				BindFunc(GetAxisInput());
		}

		InputButton UpButton = InputButton::None;
		InputButton DownButton = InputButton::None;
		std::function<void(float)> BindFunc;
	};
	
	struct InputAction {
		InputAction() = default;
		InputAction(InputButton button) : Button(button) {}

		void SetState(InputState state) { State = state; }

		template<typename T>
		void BindAction(T* object, void(T::* func)()) {
			BindFunc = [object, func]() {
				(object->*func)();
				};
		}
		template<typename T>
		void BindAction(T* object, void(T::* func)(), InputState state) {
			BindFunc = [object, func]() {
				(object->*func)();
				};

			State = state;
		}

		const InputButton GetButton() const { return Button; }

		bool GetButtonInput() const {
			// ��ư�� ���ȴ��� Ȯ��
			return ::GetAsyncKeyState(static_cast<uint32>(Button)) & 0x8000;
		}

		void UpdateInput() {
			PrevState = GetButtonInput();
		}

		// TODO: ���߿� ���ϴ� ���� ���� ���� �ٸ� ����� ������ �� �ְ� ����
		// ���ϴ� ��ư state�� ���� bool return
		bool IsState() {
			const bool currentState = GetButtonInput();
			switch (State) {
			case InputState::None:
				break;
			case InputState::Pressed:
				return !PrevState && currentState; // ������ ������ ���� && ���� ����
			case InputState::Pressing:
				return PrevState && currentState; // �������� ��� ���� ����
			case InputState::Released:
				return PrevState && !currentState; // ������ ���� ���� && ���� �ȴ���
			}
			return false;
		}

		void Execute() {
			if (BindFunc && IsState())
				BindFunc();
			UpdateInput();
		}

		InputButton Button = InputButton::None;
		std::function<void()> BindFunc;
		bool PrevState = false;
		InputState State = InputState::None;
	};
}