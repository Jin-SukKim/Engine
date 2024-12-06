#pragma once


namespace JE {

class TimeManager; 

class Engine
{
public:
	virtual void Init();
	virtual void Tick(float DeltaTime);

	const TimeManager& GetTimeManager() const { return _timeManager; }

private:
	TimeManager _timeManager;
};

}