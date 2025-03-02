//#include "../pch.h"
#include "Application.h"
#include "WindowsApp.h"
#include "Renderer/CppRenderer2D.h"
#include "Renderer/CppRenderer3D.h"

#include "Engine/AssetManager.h"
#include "Renderer/RendererInterface.h"
#include "Engine/Object.h"
#include "../MainScene.h"

namespace JE {
	std::unique_ptr<Engine> Application::_engine = std::make_unique<Engine>();

	Application::Application(const HINSTANCE& hInstance, const std::wstring& windowName, Dimension dimension) : _screenSize(800, 600), _windowName(windowName)
	{
		_hwnd = WindowsApp::Create(hInstance, windowName, _screenSize, MainProc);

		if (Dimension::DD == dimension)
			_renderer = std::make_unique<CppRenderer2D>(_hwnd);
		else if (Dimension::DDD == dimension)
			_renderer = std::make_unique<CppRenderer3D>(_hwnd);

		LoadScenes();
	}

	Application::~Application()
	{
		Destroy();
	}

	bool Application::Init(int nCmdShow)
	{
		if (!_hwnd)
			return false;

		WindowsApp::Show(_hwnd, nCmdShow);

		if (!GetRenderer()->Init(_screenSize))
			return false;

		if (!GetEngine()->Init())
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
		GetEngine()->Tick();
	}

	void Application::Render()
	{
		IRenderer* r = GetRenderer();
		r->Clear(_bgColor);
		GetEngine()->Render(r);

		r->PushStatisticText(std::format(L"{:.9f} fps", GetTimer()->GetDeltaTime()));

		r->Render(); // SwapBuffer
	}

	void Application::Destroy()
	{
		WindowsApp::Destroy(_hwnd);
	}

	TimeManager* Application::GetTimer()
	{
		if (GetEngine())
			return GetEngine()->GetTimeManager();
		return nullptr;
	}

	HINSTANCE Application::GetHInstance()
	{
		return reinterpret_cast<HINSTANCE>(GetWindowLongPtr(_hwnd, GWLP_HINSTANCE));
	}

	void Application::LoadScenes()
	{
		if (!GetEngine())
			return;

		SceneManager* manager = GetEngine()->GetSceneManager();

		manager->CreateScene<MainScene>(L"MainScene");
		manager->LoadScene(L"MainScene");
	}

	LRESULT CALLBACK MainProc(HWND hwnd, UINT32 msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg) {
		case WM_DISPLAYCHANGE:
		case WM_SIZE:
		{
			break;
		}
		case WM_CLOSE:
		{
			WindowsApp::Destroy(hwnd);
			return 0;
		}
		case WM_DESTROY:
		{
			::PostQuitMessage(0);
			return 0;
		}
		case WM_KEYDOWN:
		{
			if (wParam == VK_ESCAPE) // esc ¹öÆ°
				WindowsApp::Destroy(hwnd);
			break;
		}
		//case WM_SYSCOMMAND:
		//{
		//	if (wParam == SC_SCREENSAVE || wParam == SC_MONITORPOWER || wParam == SC_KEYMENU)
		//	{
		//		return 0;
		//	}
		//	break;
		//}
		}

		return ::DefWindowProc(hwnd, msg, wParam, lParam);
	}
}