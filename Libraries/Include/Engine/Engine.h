#pragma once


namespace JE {

class Engine
{
public:
	void Init();
	void Tick(float DeltaTime);
	void Render(HDC hdc);
};

}