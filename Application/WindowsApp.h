#pragma once
#include "pch.h"

namespace WindowsApp {
	// 윈도우 등록
	ATOM Register(const HINSTANCE& hInstance, const std::wstring& windowName, WNDPROC proc) {
		WNDCLASSEXW wcex;

		wcex.cbSize = sizeof(WNDCLASSEX);

		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = proc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		wcex.hIcon = LoadIconW(hInstance, IDI_APPLICATION);
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = windowName.c_str();
		wcex.hIconSm = LoadIconW(wcex.hInstance, MAKEINTRESOURCE(108));

		return RegisterClassExW(&wcex);
	}

	// 윈도우창 생성
	HWND Create(const HINSTANCE& hInstance, const std::wstring& windowName, const ScreenPoint& screenSize, WNDPROC proc) {
		if (!Register(hInstance, windowName, proc)) {
			MessageBoxW(0, L"RegisterClass FAILED", 0, 0);
			return NULL;
		}

		RECT rect;
		rect.left = 0;
		rect.top = 0;
		rect.right = screenSize.X - 1;
		rect.bottom = screenSize.Y - 1;
		::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false); // 타이틀바, 메뉴, 테두리 등을 제외한 크기 조정
		int windowWidth = rect.right - rect.left + 1;
		int windowHeight = rect.bottom - rect.top + 1;

		// 등록된 윈도우창 생성해 윈도우창의 핸들 반환
		HWND hwnd = ::CreateWindowW(
			windowName.c_str(),
			windowName.c_str(),
			WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX,
			// 현재 화면의 중앙에 띄우도록 위치 설정
			(GetSystemMetrics(SM_CXFULLSCREEN) - windowWidth) / 2,
			(GetSystemMetrics(SM_CYFULLSCREEN) - windowHeight) / 2,
			windowWidth,
			windowHeight,
			NULL,
			NULL,
			hInstance,
			NULL
		);

		if (!hwnd) {
			::MessageBox(0, L"Create Window Failed", 0, 0);
			return NULL;
		}

		return hwnd;
	}

	// 생성한 윈도우창 화면에 표시하고 갱싱
	void Show(HWND hWnd, int nCmdShow) {
		::ShowWindow(hWnd, nCmdShow);
		::UpdateWindow(hWnd);
	}

	void Destroy(HWND hWnd) {
		::DestroyWindow(hWnd);
	}

	// 윈도우의 입력을 받는 Loop
	FORCEINLINE bool Tick(HINSTANCE hInstance) {
		MSG msg;
		::ZeroMemory(&msg, sizeof(msg));

		// PeekMessage: 메시지가 있다면 true, 없다면 false return
		while (::PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT)
				return false;

			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}

		return true;
	}

	LRESULT CALLBACK Proc(HWND hwnd, UINT32 msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg) {
		case WM_DISPLAYCHANGE:
		case WM_SIZE:
		{
			break;
		}
		case WM_CLOSE:
		{
			Destroy(hwnd);
			return 0;
		}
		case WM_DESTROY:
		{
			::PostQuitMessage(0);
			return 0;
		}
		case WM_KEYDOWN:
		{
			if (wParam == VK_ESCAPE) // esc 버튼
				Destroy(hwnd);
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