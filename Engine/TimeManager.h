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
	LARGE_INTEGER _freq = {}; // ���� ī������ 1�� ������ �߻� �󵵼�
	LARGE_INTEGER _prevCount = {}; // ���� CPU Ŭ����
	LARGE_INTEGER _currentCount = {};
	float _deltaTime = 0.f; // �� frame���� ���� frame���� �ɸ� �ð�

	uint32 _frameCount = 0; // Frame�� ��
	float _frameTime = 0.f; // ����� �� �ð� 
	float _fps = 0.f; // frame/sec
};
}

