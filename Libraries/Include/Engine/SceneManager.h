#pragma once
#include "Scene.h"

namespace JE {
	class IRenderer;
	class Object;
	class CollisionManager;

	class SceneManager
	{
	public:
		void Init();
		void Tick(const float& DeltaTime);
		void Render(IRenderer* r);
		void Destroy();

		// Scene 생성
		template<typename T>
		Scene* CreateScene(const std::wstring& name);
		// 생성한 Scene 가져오기
		Scene* LoadScene(const std::wstring& name);
		// 활성화된 Scene 가져오기
		Scene* GetActiveScene() const { return _activeScene; }
		// Layer의 모든 Object 가져오기 (Collision에서 주로 사용)
		const std::vector<std::unique_ptr<Object>>* GetObjects(LayerType layer);
		void SetCollisionManager(CollisionManager* manager) { _collisionManager = manager; }
	private:
		std::map<const std::wstring, std::unique_ptr<Scene>> _scenes;
		Scene* _activeScene = nullptr; // 현재 화면에 그려질 Scene
		CollisionManager* _collisionManager = nullptr;
	};

	template<typename T>
	inline Scene* SceneManager::CreateScene(const std::wstring& name)
	{
		std::unique_ptr<T> newScene = std::make_unique<T>(name);
		_scenes[name] = std::move(newScene);
		_activeScene = _scenes[name].get();

		_activeScene->Init();
		return _activeScene;
	}

}