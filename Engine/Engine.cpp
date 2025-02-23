#include "pch.h"
#include "Engine.h"
#include "TimeManager.h"
#include "CollisionManager.h"

namespace JE {
	bool Engine::Init()
	{
		if (!_timeManager.Init())
			return false;

		SetManager();
		GetCollisionManager()->Init();

		return true;
	}

	void Engine::Tick()
	{
		GetInputManager()->UpdateInput();
		_collisionManager.Tick();
		_sceneManager.Tick(_timeManager.GetDeltaTime());
	}

	void Engine::Render(IRenderer* r)
	{
		_sceneManager.Render(r);
	}
	void Engine::SetManager()
	{
		_sceneManager.SetCollisionManager(GetCollisionManager());
		_collisionManager.SetSceneManager(GetSceneManager());
	}
};