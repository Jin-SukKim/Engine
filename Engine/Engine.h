#pragma once


namespace JE {

class TimeManager; 
class IRenderer;
class Engine
{
public:
	virtual ~Engine() {};

	virtual bool Init();
	virtual void Tick(float DeltaTime);
	virtual void Render(IRenderer* r);

	TimeManager* GetTimeManager() { return &_timeManager; }
	InputManager* GetInputManager() { return &_inputManager; }
private:
	TimeManager _timeManager;
	// TODO: InputManager�� Engine�� �ƴ� �� Window Ŭ������ ������ �־�� ��
	InputManager _inputManager;
};

}