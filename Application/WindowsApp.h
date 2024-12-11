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
		case WM_PAINT:
		{
			// DC란 화면에 출력에 필요한 모든 정보를 가지는 데이터 구조체이며
			// GDI 모듈에 의해 관리 (폰트, 선의 굵기, 색상 등)
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);

			// 색상 변경
			HBRUSH brush = CreateSolidBrush(RGB(255, 0, 255)); // brush 생성(메모리에 할당)
			// 생성한 brush를 사용하도록 변경하고 기존의 brush를 저장해두기
			HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush); // 사용하던 brush 반환

			Rectangle(hdc, 100, 100, 200, 200);

			SelectObject(hdc, oldBrush); // 다시 원래 brush로 변경
			DeleteObject(brush); // 생성한 brush는 이제 사용하지 않으므로 메모리에서 삭제

			// 펜 생성
			HPEN redPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
			HPEN oldPen = (HPEN)SelectObject(hdc, redPen);

			Ellipse(hdc, 200, 200, 300, 300);

			SelectObject(hdc, oldPen);
			DeleteObject(redPen);

			EndPaint(hwnd, &ps);
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