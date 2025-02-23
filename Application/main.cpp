#include "pch.h"
#include "WindowsApp.h"
#include "Application.h"
#include "Renderer/CppRenderer2D.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    // TODO: renderer 생성을 Application 안에서 하기 (추후 다른 window를 생성하기 위함), Engine은 창이 여러개라도 하나만 쓰면 되지 않을까
    ScreenPoint screenSize(800, 600);
    //Application app(screenSize);
    // 메인 윈도우
    HWND hwnd = WindowsApp::Create(hInstance, L"GameEngine", screenSize, WindowsApp::Proc);
    if (!hwnd)
        return -1;

    // Engine 객체 생성
    std::unique_ptr<Engine> engine = std::make_unique<Engine>();
    std::unique_ptr<IRenderer> renderer = std::make_unique<CppRenderer2D>(hwnd);
    Application app(std::move(engine), std::move(renderer));
    if (!app.Init(screenSize))
        return -1;

    WindowsApp::Show(hwnd, nCmdShow);

    while (WindowsApp::Tick(hInstance)) {
        // Engine Logic
        app.Run();
    }

    WindowsApp::Destroy(hwnd);
    return 0;
}