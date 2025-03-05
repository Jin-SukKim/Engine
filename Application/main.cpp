#include "pch.h"
#include "Application.h"
#include "./Windows/WindowsUtils.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    // 메인 윈도우
    Application app(hInstance);
    if (!app.Init(nCmdShow))
        return -1;

    while (WindowsUtils::Tick()) {
        // Engine Logic
        app.Run();
    }

    return 0;
}