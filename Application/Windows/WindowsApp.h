#pragma once

namespace JE {
	class WindowsApp
	{
	public:
		enum class Dimension {
			DD,
			DDD
		};

		WindowsApp(const HINSTANCE& hInstance, const std::wstring& windowName, Dimension dimension);
		virtual ~WindowsApp();

		virtual bool Init(int nCmdShow);
		virtual void Render();
		void Destroy();

		IRenderer* GetRenderer() { return _renderer.get(); }

		HINSTANCE GetHInstance();
		const std::wstring& GetWindowName() { return _windowName; }

		virtual void LoadScenes();

		virtual LRESULT CALLBACK Proc(HWND hwnd, UINT32 msg, WPARAM wParam, LPARAM lParam);

	protected:
		ScreenPoint _screenSize;
		HWND _hwnd = {};
		std::wstring _windowName;

		std::unique_ptr<IRenderer> _renderer;
		Color _bgColor = Color::White;
	};
}