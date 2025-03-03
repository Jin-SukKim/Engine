//#include "../pch.h"
#include "Application.h"
#include "WindowsUtils.h"
#include "Renderer/CppRenderer2D.h"
#include "Renderer/CppRenderer3D.h"

#include "Engine/AssetManager.h"
#include "Renderer/RendererInterface.h"
#include "Engine/Object.h"
#include "../MainScene.h"
#include "../WindowsApp.h"

namespace JE {
	std::map<std::wstring, std::unique_ptr<WindowsApp>> Application::_windows = {};

	Application::Application(const HINSTANCE& hInstance)
	{
		AddWindow(hInstance, L"MainWindow", WindowsApp::Dimension::DD);
		//AddWindow(hInstance, L"SubWindow", WindowsApp::Dimension::DD);
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

	void Application::LoadScenes()
	{
		SceneManager* manager = Engine::GetEngine().GetSceneManager();

		manager->CreateScene<MainScene>(L"MainScene");
		manager->LoadScene(L"MainScene");
	}

	void Application::AddWindow(const HINSTANCE& hIstance, const std::wstring& windowName, WindowsApp::Dimension dim)
	{
		auto iter = _windows.find(windowName);
		if (iter != _windows.end())
			return;
		std::unique_ptr<WindowsApp> window = std::make_unique<WindowsApp>(hIstance, windowName, dim);
		_windows[windowName] = std::move(window);
	}
}