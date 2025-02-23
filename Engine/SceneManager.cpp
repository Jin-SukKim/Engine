#include "pch.h"
#include "SceneManager.h"
#include "Object.h"
#include "Layer.h"

namespace JE {
	void SceneManager::Init()
	{
		for (auto& [key, value] : _scenes) {
			Scene* scene = value.get();
			if (scene)
				scene->Init();
		}
	}
	void SceneManager::Tick(const float& DeltaTime)
	{
		// Ȱ��ȭ�� Scene�� ����
		if (_activeScene)
			_activeScene->Tick(DeltaTime);
	}
	void SceneManager::Render(IRenderer* r)
	{
		if (_activeScene)
			_activeScene->Render(r);
	}

	void SceneManager::Destroy()
	{
		if (_activeScene)
			_activeScene->Destroy();
	}
	Scene* SceneManager::LoadScene(const std::wstring& name)
	{
		// ���� Scene���� ������
		if (_activeScene) {
			// �浹 �ʱ�ȭ
			if (_collisionManager) {
				_collisionManager->Clear();
				_collisionManager->Init();
			}

			_activeScene->OnExit();
		}
		// ���� Scene ã��
		std::map<const std::wstring, std::unique_ptr<Scene>>::iterator iter = _scenes.find(name);
		// ã�� Scene�� ���ٸ�
		if (iter == _scenes.end())
			return nullptr;

		// ���� Scene�� ���� Scene���� ���
		_activeScene = (iter->second).get();
		_activeScene->OnEnter(); // Scene�� ����

		return _activeScene;
	}

	const std::vector<std::unique_ptr<Object>>* SceneManager::GetObjects(LayerType layer)
	{
		/*const std::vector<std::unique_ptr<Object>>& activeObjects = _activeScene->GetLayer(layer)->GetObjects();
		for (const std::unique_ptr<Object>& obj : activeObjects)
			objects.emplace_back(obj.get());*/
		if (!_activeScene)
			return nullptr;

		return &_activeScene->GetLayer(layer)->GetObjects();
	}
}