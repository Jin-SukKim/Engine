#include "pch.h"
#include "CollisionManager.h"

namespace JE {
	void CollisionManager::Init()
	{
		// �⺻���� �浹 ����
		if (_sceneManager) {
			SetLayerCollision(LayerType::Actor, LayerType::Floor, true);
			SetLayerCollision(LayerType::Player, LayerType::Floor, true);
			SetLayerCollision(LayerType::Player, LayerType::Enemy, true);
			SetLayerCollision(LayerType::Enemy, LayerType::Floor, true);
		}
	}

	void CollisionManager::Tick()
	{
		if (!_sceneManager)
			return;

		Scene* scene = _sceneManager->GetActiveScene();
		uint16 maxLayer = static_cast<uint16>(LayerType::MaxLayerType);
		for (uint16 row = 0; row < maxLayer; ++row) {
			for (uint16 col = 0; col < maxLayer; ++col) {
				// �浹�ص� �Ǵ� layer���� Ȯ��
				if (_collisionLayerMatrix[row][col]) {
					LayerCollision(scene, static_cast<LayerType>(row), static_cast<LayerType>(col));
				}
			}
		}
	}

	void CollisionManager::Clear()
	{
		_collisionMap.clear();
		_collisionLayerMatrix->reset();
	}

	void CollisionManager::SetLayerCollision(LayerType left, LayerType right, bool enable)
	{
		uint16 row = 0, col = 0;

		// layertype�� ���� �������� �켱������ ����
		// ��, row�� col�� �ε������� ����
		if (left <= right) {
			row = static_cast<uint16>(left);
			col = static_cast<uint16>(right);
		}
		else {
			row = static_cast<uint16>(right);
			col = static_cast<uint16>(left);
		}

		_collisionLayerMatrix[row][col] = enable;
	}

	void CollisionManager::LayerCollision(Scene* scene, LayerType left, LayerType right)
	{
		// �� layer�� objects ��������
		const std::vector<std::unique_ptr<Object>>* lefts = _sceneManager->GetObjects(left);
		const std::vector<std::unique_ptr<Object>>* rights = _sceneManager->GetObjects(right);
		if (!lefts || !rights)
			return;

		for (uint16 i = 0; i < lefts->size(); ++i) {
			Object* left = (*lefts)[i].get();
			if (!left || !left->IsActive())
				continue;

			Collider* leftCollider = left->GetComponent<Collider>();
			if (!leftCollider)
				continue;

			for (uint16 j = 0; j < rights->size(); ++j) {
				Object* right = (*rights)[j].get();
				if (!right || !right->IsActive())
					continue;

				Collider* rightCollider = right->GetComponent<Collider>();
				if (!rightCollider || left == right)
					continue;

				// �� ������Ʈ�� �浹 Ȯ��
				CheckCollision(leftCollider, rightCollider);
			}
		}
	}

	void CollisionManager::CheckCollision(Collider* left, Collider* right)
	{
		// �� �浹ü ��ȣ�� ������ ID�� Ȯ���� CollisionID���� ����
		CollisionID collisionID = {};
		collisionID.Left = left->GetID();
		collisionID.Right = right->GetID();

		// �ش� ID�� �浹ü ������ �˻�
		auto iter = _collisionMap.find(collisionID.ID);
		// ���ٸ� �浹���� ����
		if (iter == _collisionMap.end()) {
			_collisionMap[collisionID.ID] = false;
			iter = _collisionMap.find(collisionID.ID);
		}

		// �浹üũ
		if (left->CheckCollision(right)) {
			// ������ �浹���� �ʾҴٸ�
			if (!iter->second) {
				left->OnCollisionBeginOverlap(right);
				right->OnCollisionBeginOverlap(left);
				iter->second = true;
			}
			// �浹��
			else {
				left->OnCollisionOverlap(right);
				right->OnCollisionOverlap(left);
			}
		}
		// �浹���� �ʴ´ٸ�
		else {
			// ������ �浹 ���̾��ٸ�
			if (iter->second) {
				left->OnCollisionEndOverlap(right);
				right->OnCollisionEndOverlap(left);
				iter->second = false;
			}
		}
	}
}