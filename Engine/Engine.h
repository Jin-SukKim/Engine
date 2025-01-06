#pragma once


namespace JE {

class TimeManager; 

class Engine
{
public:
	virtual ~Engine() {};

	virtual bool Init();
	virtual void Tick(float DeltaTime);
	virtual void Render();

	TimeManager* GetTimeManager() { return &_timeManager; }
	InputManager* GetInputManager() { return &_inputManager; }
private:
	TimeManager _timeManager;
	// TODO: InputManager를 Engine이 아닌 각 Window 클래스가 가지고 있어야 함
	InputManager _inputManager;
};

}