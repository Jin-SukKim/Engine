#pragma once

class TimeManager;

namespace JE {

class Engine
{
public:
	void Init();
	void Tick(float DeltaTime);
	void Render(HDC hdc);

private:
	std::unique_ptr<TimeManager> _timeManager;
};

}