#pragma once
#include "../pch.h"

namespace JE {
	class SpriteActor;
	class Mesh2DComponent;
	class Camera2DComponent;
	class Object;

	class Application
	{
	public:
		enum class Dimension {
			DD,
			DDD
		};

		Application(const HINSTANCE& hInstance, const std::wstring& windowName, Dimension dimension);
		virtual ~Application();
		virtual bool Init(int nCmdShow);
		void Run();
		virtual void Tick();
		virtual void Render();
		void Destroy();

		Engine* GetEngine() { return _engine.get(); }
		TimeManager* GetTimer();
		IRenderer* GetRenderer() { return _renderer.get(); }

		HINSTANCE GetHInstance();
		const std::wstring& GetWindowName() { return _windowName; }

		template<typename T>
		static T* Instantiate(const std::wstring& name, LayerType type);
		
		void LoadScenes();
	protected:
		ScreenPoint _screenSize;
		HWND _hwnd = {};
		std::wstring _windowName;

		static std::unique_ptr<Engine> _engine;
		std::unique_ptr<IRenderer> _renderer;
		Color _bgColor = Color::White;
	};

	LRESULT CALLBACK MainProc(HWND hwnd, UINT32 msg, WPARAM wParam, LPARAM lParam);

	template<typename T>
	inline T* Application::Instantiate(const std::wstring& name, LayerType type)
	{
		std::unique_ptr<T> newObj = std::make_unique<T>(name);
		newObj->SetLayerType(type);
		Scene* activeScene = _engine->GetSceneManager()->GetActiveScene();
		if (!activeScene)
			return nullptr;
		Layer* layer = activeScene->GetLayer(type);
		if (!layer)
			return nullptr;

		return dynamic_cast<T*>(layer->AddObject(std::move(newObj)));
	}
}
