#pragma once

namespace JE {

class Application
{
public:
	void Init();
	bool Tick();
	void Render();

private:
	HWND _hwnd;
};

}