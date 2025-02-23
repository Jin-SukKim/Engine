#pragma once
#include "Collider.h"

namespace JE {
	// 충돌한 오브젝트 관리를 위한 ID 구조체
	union CollisionID {
		struct {
			uint32 Left;
			uint32 Right;
		};
		uint64 ID;
	};

	class CollisionManager
	{
	public:
		void Init();
		void Tick();
		void Clear();

		// 어떤 Layer와 충돌해도 될지 설정
		void SetLayerCollision(LayerType left, LayerType right, bool enable);
		// Layer별로 충돌해도 되는 Layer인지 확인
		void LayerCollision(class Scene* scene, LayerType left, LayerType right);
		// Collision 확인
		void CheckCollision(Collider* left, Collider* right);
		// CollisionManager에서 사용할 SceneManager 설정
		void SetSceneManager(SceneManager* manager) { _sceneManager = manager; }
	private:
		// bit 연산과 디버깅을 쉽게하기 위한 자료형
		std::bitset<static_cast<uint16>(LayerType::MaxLayerType)> _collisionLayerMatrix[static_cast<uint16>(LayerType::MaxLayerType)];
		// 두 충돌체의 충돌 여부 저장
		std::unordered_map<uint64, bool> _collisionMap;
		SceneManager* _sceneManager = nullptr;
	};
}
