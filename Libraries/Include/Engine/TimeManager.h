#pragma once

namespace {

	
class TimeManager
{
public:
	void Init();
	void Begin();
	void End();

	float GetFPS() const {
		return _fps;
	}
	float GetDeltaTime() const {
		return _deltaTime;
	}

private:
	uint64 _freq = 0;
	uint64 _prevCount = 0;
	float _deltaTime = 0.f;

	uint64 _frameCount = 0;
	float _frameTime = 0.f;
	float _fps = 0.f;
};
}

