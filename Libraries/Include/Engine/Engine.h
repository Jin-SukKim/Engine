#pragma once


namespace JE {

class TimeManager; 

class Engine
{
public:
	void Init();
	void Tick(float DeltaTime);
	void Render(HDC hdc);

	const TimeManager& GetTimeManager() const { return _timeManager; }

private:
	TimeManager _timeManager;
};

}