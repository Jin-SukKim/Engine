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

			// XOR로 둘 중 하나만 true면 pass
			if (isUp ^ isDown)
				return isDown ? -1.f : 1.f;
				
			// 둘 다 false이거나 true인 경우
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
			// 버튼이 눌렸는지 확인
			return ::GetAsyncKeyState(static_cast<uint32>(Button)) & 0x8000;
		}

		void UpdateInput() {
			PrevState = GetButtonInput();
		}

		// TODO: 나중에 원하는 상태 별로 각각 다른 기능을 수행할 수 있게 변경
		// 원하는 버튼 state에 따라 bool return
		bool IsState() {
			const bool currentState = GetButtonInput();
			switch (State) {
			case InputState::None:
				break;
			case InputState::Pressed:
				return !PrevState && currentState; // 이전에 눌린적 없음 && 현재 눌림
			case InputState::Pressing:
				return PrevState && currentState; // 이전부터 계속 눌린 상태
			case InputState::Released:
				return PrevState && !currentState; // 이전엔 눌린 상태 && 현재 안눌림
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