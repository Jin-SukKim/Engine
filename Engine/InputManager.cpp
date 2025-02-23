#include "pch.h"
#include "InputManager.h"

namespace JE {
    void InputManager::UpdateInput()
    {
        // �Ź� Ű������ �Է��� �޾� Update
        for (InputAxisAction& action : _axisMap)
            action.Execute();

        for (InputAction& action : _buttonMap)
            action.Execute();
    }

    float InputManager::GetAxis(InputAxis inputAxis) const
    {
        if (inputAxis < InputAxis::MaxAxis) {
            InputAxisAction action = _axisMap[static_cast<size_t>(inputAxis)];
            return action.GetAxisInput();
        }

        return 0.0f;
    }

    bool InputManager::GetButton(InputButton button) const
    {
        if (button < InputButton::MaxButtons) {
            // Button�� ���¿� ���� true/false return
            InputAction action = _buttonMap[static_cast<size_t>(button)];
            return action.IsState();
        }
        return false;
    }
}