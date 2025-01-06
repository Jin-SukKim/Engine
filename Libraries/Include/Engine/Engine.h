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
	// TODO: InputManager�� Engine�� �ƴ� �� Window Ŭ������ ������ �־�� ��
	InputManager _inputManager;
};

}