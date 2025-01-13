#pragma once


namespace JE {

class TimeManager; 
class IRenderer;
class SceneManager;

class Engine
{
public:
	virtual ~Engine() {};

	virtual bool Init();
	virtual void Tick(float DeltaTime);
	virtual void Render(IRenderer* r);

	TimeManager* GetTimeManager() { return &_timeManager; }
	InputManager* GetInputManager() { return &_inputManager; }
	SceneManager* GetSceneManager() { return &_sceneManager; }
private:
	TimeManager _timeManager;
	// TODO: InputManager를 Engine이 아닌 각 Window 클래스가 가지고 있어야 함
	InputManager _inputManager;
	SceneManager _sceneManager; // 각 윈도우가 가질지 엔진에 하나의 Manager만 가질지 고민 중
};

}