#pragma once

namespace JE {
	enum class InputAxis {
		XAxis = 0,
		YAxis,
		ZAxis,
		WAxis,
		MaxAxis // InputAxis ����
	};

	enum class InputButton {
		None,
		Up = VK_UP,
		Down = VK_DOWN,
		Left = VK_LEFT,
		Right = VK_RIGHT,
		PgUp = VK_PRIOR,
		PgDown = VK_NEXT,
		Home = VK_HOME,
		End = VK_END,
		W = 0x57,
		A = 0x41,
		S = 0x53,
		D = 'D',
		F1 = VK_F1,
		F2 = VK_F2,
		F3 = VK_F3,
		F4 = VK_F4,
		F5 = VK_F5,
		MaxButtons // InputButton ����
	};

	enum class InputState {
		None,
		Pressed,
		Pressing,
		Released
	};
}