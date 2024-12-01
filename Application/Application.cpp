#include "pch.h"
#include "WindowsApp.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    ScreenPoint screenSize(800, 600);
    // 메인 윈도우
    HWND hwnd = WindowsApp::Create(hInstance, L"GameEngine", screenSize, WindowsApp::Proc);
    if (!hwnd)
        return -1;

    // TODO: Engine 객체 생성

    WindowsApp::Show(hwnd, nCmdShow);

    while (WindowsApp::Tick(hInstance)) {
        // Engine Logic
    }

    WindowsApp::Destroy(hwnd);
    return 0;
}