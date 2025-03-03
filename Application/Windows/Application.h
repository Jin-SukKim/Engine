#pragma once
#include "../pch.h"
#include "../WindowsApp.h"

namespace JE {

	class Application
	{
	public:
		Application(const HINSTANCE& hInstance);
		virtual ~Application();
		virtual bool Init(int nCmdShow);
		void Run();
		virtual void Tick();
		virtual void Render();
		void Destroy();

		TimeManager* GetTimer();
		void LoadScenes();

		static void AddWindow(const HINSTANCE& hIstance, const std::wstring& windowName, WindowsApp::Dimension dim);
	protected:
		static std::map<std::wstring, std::unique_ptr<WindowsApp>> _windows;
	};
}
