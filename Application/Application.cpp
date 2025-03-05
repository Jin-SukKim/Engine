#include "pch.h"
#include "Application.h"
#include "Windows/MainEditor.h"

namespace JE {
	std::map<std::wstring, std::unique_ptr<WindowsApp>> Application::_windows = {};

	Application::Application(const HINSTANCE& hInstance)
	{
		AddWindow<MainEditor>(hInstance, L"MainWindow", WindowsApp::Dimension::DD);
	}

	Application::~Application()
	{
		Destroy();
	}

	bool Application::Init(int nCmdShow)
	{
		if (!Engine::GetEngine().Init())
			return false;

		for (const auto& [key, win] : _windows)
			if (!win->Init(nCmdShow))
				return false;

		return true;
	}

	void Application::Run()
	{
		GetTimer()->Begin();

		Tick();
		Render();

		GetTimer()->End();
	}

	void Application::Tick()
	{
		Engine::GetEngine().Tick();
	}

	void Application::Render()
	{
		for (const auto& [key, win] : _windows)
			win->Render();
	}

	void Application::Destroy()
	{
		for (const auto& [key, win] : _windows)
			win->Destroy();
	}

	TimeManager* Application::GetTimer()
	{
		return Engine::GetEngine().GetTimeManager();
	}
}