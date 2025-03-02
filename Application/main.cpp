#include "pch.h"
#include "./Windows/WindowsApp.h"
#include "./Windows/Application.h"
#include "Renderer/CppRenderer2D.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    // 메인 윈도우
    Application app(hInstance, L"GameEngine", Application::Dimension::DD);
    if (!app.Init(nCmdShow))
        return -1;

    while (WindowsApp::Tick()) {
        // Engine Logic
        app.Run();
    }

    return 0;
}