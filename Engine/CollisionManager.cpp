#include "pch.h"
#include "CollisionManager.h"

namespace JE {
	void CollisionManager::Init()
	{
		// 기본적인 충돌 설정
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
				// 충돌해도 되는 layer인지 확인
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

		// layertype의 값이 낮을수록 우선순위가 높음
		// 즉, row가 col에 부딪히는지 설정
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
		// 각 layer의 objects 가져오기
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

				// 두 오브젝트의 충돌 확인
				CheckCollision(leftCollider, rightCollider);
			}
		}
	}

	void CollisionManager::CheckCollision(Collider* left, Collider* right)
	{
		// 두 충돌체 번호를 가져온 ID를 확인해 CollisionID값을 세팅
		CollisionID collisionID = {};
		collisionID.Left = left->GetID();
		collisionID.Right = right->GetID();

		// 해당 ID로 충돌체 정보를 검색
		auto iter = _collisionMap.find(collisionID.ID);
		// 없다면 충돌정보 생성
		if (iter == _collisionMap.end()) {
			_collisionMap[collisionID.ID] = false;
			iter = _collisionMap.find(collisionID.ID);
		}

		// 충돌체크
		if (left->CheckCollision(right)) {
			// 이전에 충돌하지 않았다면
			if (!iter->second) {
				left->OnCollisionBeginOverlap(right);
				right->OnCollisionBeginOverlap(left);
				iter->second = true;
			}
			// 충돌중
			else {
				left->OnCollisionOverlap(right);
				right->OnCollisionOverlap(left);
			}
		}
		// 충돌하지 않는다면
		else {
			// 이전에 충돌 중이었다면
			if (iter->second) {
				left->OnCollisionEndOverlap(right);
				right->OnCollisionEndOverlap(left);
				iter->second = false;
			}
		}
	}
}