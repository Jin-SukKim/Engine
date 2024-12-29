#include "pch.h"
#include "Engine.h"
#include "TimeManager.h"

namespace JE {
	bool Engine::Init()
	{
		if (!GetTimeManager()->Init())
			return false;

		return true;
	}

	void Engine::Tick(float DeltaTime)
	{
		GetInputManager()->UpdateInput();
	}

	void Engine::Render()
	{
	}
};