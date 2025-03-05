#include "../pch.h"
#include "MainEditor.h"

#include "../Scene/MainScene.h"

namespace JE {
	MainEditor::MainEditor(const HINSTANCE& hInstance, const std::wstring& windowName, Dimension dimension)
		: Super(hInstance, windowName, dimension)
	{
		LoadScenes();
	}

	MainEditor::~MainEditor()
	{
		Destroy();
	}

	bool MainEditor::Init(int nCmdShow)
	{
		Super::Init(nCmdShow);

		return true;
	}

	void MainEditor::Render()
	{
		_renderer->Clear(_bgColor);
		Engine::GetEngine().Render(_renderer.get());

		_renderer->PushStatisticText(std::format(L"{:.9f} fps", Engine::GetEngine().GetTimeManager()->GetDeltaTime()));

		_renderer->Render(); // SwapBuffer
	}

	void MainEditor::LoadScenes()
	{
		SceneManager* manager = Engine::GetEngine().GetSceneManager();

		manager->CreateScene<MainScene>(L"MainScene");
		manager->LoadScene(L"MainScene");
	}
	LRESULT MainEditor::Proc(HWND hwnd, UINT32 msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg) {
		case WM_DISPLAYCHANGE:
		case WM_SIZE:
		{
			break;
		}
		case WM_CLOSE:
		{
			Destroy();
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
				Destroy();
			break;
		}
		}

		return ::DefWindowProc(hwnd, msg, wParam, lParam);
	}
}