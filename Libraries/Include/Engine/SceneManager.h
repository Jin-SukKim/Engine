#pragma once
#include "Scene.h"

namespace JE {
	class IRenderer;
	class Object;

	class SceneManager
	{
	public:
		void Init();
		void Tick(const float& DeltaTime);
		void Render(IRenderer* r);
		void Destroy();

		// Scene ����
		template<typename T>
		Scene* CreateScene(const std::wstring& name);
		// ������ Scene ��������
		Scene* LoadScene(const std::wstring& name);

		// Layer�� ��� Object �������� (Collision���� �ַ� ���)
		std::vector<Object*> GetObjects(LayerType layer);
	private:
		std::map<const std::wstring, std::unique_ptr<Scene>> _scenes;
		Scene* _activeScene = nullptr; // ���� ȭ�鿡 �׷��� Scene
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