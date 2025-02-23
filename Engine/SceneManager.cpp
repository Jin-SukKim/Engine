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
		// 활성화된 Scene만 실행
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
		// 현재 Scene에서 나가기
		if (_activeScene) {
			// 충돌 초기화
			if (_collisionManager) {
				_collisionManager->Clear();
				_collisionManager->Init();
			}

			_activeScene->OnExit();
		}
		// 다음 Scene 찾기
		std::map<const std::wstring, std::unique_ptr<Scene>>::iterator iter = _scenes.find(name);
		// 찾는 Scene이 없다면
		if (iter == _scenes.end())
			return nullptr;

		// 다음 Scene을 현재 Scene으로 사용
		_activeScene = (iter->second).get();
		_activeScene->OnEnter(); // Scene에 진입

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