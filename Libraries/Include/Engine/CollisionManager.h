#pragma once
#include "Collider.h"

namespace JE {
	// �浹�� ������Ʈ ������ ���� ID ����ü
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

		// � Layer�� �浹�ص� ���� ����
		void SetLayerCollision(LayerType left, LayerType right, bool enable);
		// Layer���� �浹�ص� �Ǵ� Layer���� Ȯ��
		void LayerCollision(class Scene* scene, LayerType left, LayerType right);
		// Collision Ȯ��
		void CheckCollision(Collider* left, Collider* right);
		// CollisionManager���� ����� SceneManager ����
		void SetSceneManager(SceneManager* manager) { _sceneManager = manager; }
	private:
		// bit ����� ������� �����ϱ� ���� �ڷ���
		std::bitset<static_cast<uint16>(LayerType::MaxLayerType)> _collisionLayerMatrix[static_cast<uint16>(LayerType::MaxLayerType)];
		// �� �浹ü�� �浹 ���� ����
		std::unordered_map<uint64, bool> _collisionMap;
		SceneManager* _sceneManager = nullptr;
	};
}
