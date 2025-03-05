#pragma once
#include "Windows/WindowsApp.h"

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

		template<typename T>
		static void AddWindow(const HINSTANCE& hIstance, const std::wstring& windowName, WindowsApp::Dimension dim);
	protected:
		static std::map<std::wstring, std::unique_ptr<WindowsApp>> _windows;
	};

	template<typename T>
	inline void Application::AddWindow(const HINSTANCE& hIstance, const std::wstring& windowName, WindowsApp::Dimension dim)
	{
		auto iter = _windows.find(windowName);
		if (iter != _windows.end())
			return;
		std::unique_ptr<T> window = std::make_unique<T>(hIstance, windowName, dim);
		_windows[windowName] = std::move(window);
	}
}
