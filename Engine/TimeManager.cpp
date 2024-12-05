#include "pch.h"
#include "TimeManager.h"

using namespace JE;

bool TimeManager::Init()
{
	if (!::QueryPerformanceFrequency(&_freq))
		return false;

	return true;
}

void TimeManager::Begin()
{
	::QueryPerformanceCounter(&_prevCount);
}

void TimeManager::End()
{
	::QueryPerformanceCounter(&_currentCount);

	_deltaTime = static_cast<float>(_currentCount.QuadPart - _prevCount.QuadPart) / static_cast<float>(_freq.QuadPart);

	++_frameCount;
	_frameTime += _deltaTime;


	if (_frameTime >= 1.f) {
		_fps = _frameCount / _frameTime;

		_frameCount = 0;
		_frameTime = 0;
	}
}
