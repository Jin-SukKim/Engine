#include "pch.h"
#include "WindowsApp.h"
#include "Application.h"
#include "Renderer/CppRenderer2D.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    // TODO: renderer ������ Application �ȿ��� �ϱ� (���� �ٸ� window�� �����ϱ� ����), Engine�� â�� �������� �ϳ��� ���� ���� ������
    ScreenPoint screenSize(800, 600);
    //Application app(screenSize);
    // ���� ������
    HWND hwnd = WindowsApp::Create(hInstance, L"GameEngine", screenSize, WindowsApp::Proc);
    if (!hwnd)
        return -1;

    // Engine ��ü ����
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