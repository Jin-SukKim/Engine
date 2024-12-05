#pragma once

namespace JE {

	
class TimeManager
{
public:
	bool Init();
	void Begin();
	void End();

	float GetFPS() const {
		return _fps;
	}
	float GetDeltaTime() const {
		return _deltaTime;
	}

private:
	LARGE_INTEGER _freq = {}; // 성능 카운터의 1초 동안의 발생 빈도수
	LARGE_INTEGER _prevCount = {}; // 이전 CPU 클락값
	LARGE_INTEGER _currentCount = {};
	float _deltaTime = 0.f; // 전 frame에서 다음 frame까지 걸린 시간

	uint32 _frameCount = 0; // Frame의 수
	float _frameTime = 0.f; // 경과된 총 시간 
	float _fps = 0.f; // frame/sec
};
}

