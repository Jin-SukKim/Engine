#include "../pch.h"
#include "WindowsApp.h"
#include "WindowsUtils.h"
#include "Renderer/CppRenderer2D.h"
#include "Renderer/CppRenderer3D.h"

namespace JE {
	WindowsApp::WindowsApp(const HINSTANCE& hInstance, const std::wstring& windowName, Dimension dimension) : _screenSize(800, 600), _windowName(windowName)
	{
		_hwnd = WindowsUtils::Create(hInstance, windowName, _screenSize, [](HWND hwnd, UINT32 msg, WPARAM wParam, LPARAM lParam) -> LRESULT {
			WindowsApp* app = reinterpret_cast<WindowsApp*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
			if (app) {
				return app->Proc(hwnd, msg, wParam, lParam);
			}
			return DefWindowProc(hwnd, msg, wParam, lParam);
			});

		SetWindowLongPtr(_hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

		if (Dimension::DD == dimension)
			_renderer = std::make_unique<CppRenderer2D>(_hwnd);
		else if (Dimension::DDD == dimension)
			_renderer = std::make_unique<CppRenderer3D>(_hwnd);
		
		LoadScenes();
	}

	WindowsApp::~WindowsApp()
	{
		Destroy();
	}

	bool WindowsApp::Init(int nCmdShow)
	{
		if (!_hwnd)
			return false;

		WindowsUtils::Show(_hwnd, nCmdShow);

		if (!GetRenderer()->Init(_screenSize))
			return false;

		return true;
	}

	void WindowsApp::Render()
	{
		_renderer->Clear(_bgColor);

		_renderer->Render(); // SwapBuffer
	}

	void WindowsApp::Destroy()
	{
		WindowsUtils::Destroy(_hwnd);
	}

	HINSTANCE WindowsApp::GetHInstance()
	{
		return reinterpret_cast<HINSTANCE>(GetWindowLongPtr(_hwnd, GWLP_HINSTANCE));
	}

	void WindowsApp::LoadScenes()
	{
	}

	LRESULT CALLBACK WindowsApp::Proc(HWND hwnd, UINT32 msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg) {
		case WM_DISPLAYCHANGE:
		case WM_SIZE:
		{
			break;
		}
		case WM_CLOSE:
		{
			WindowsUtils::Destroy(hwnd);
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
				WindowsUtils::Destroy(hwnd);
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
