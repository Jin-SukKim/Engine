#pragma once

namespace JE {
	// �� ������Ʈ�� ������ �ִ� Component�� Update ������ �����ϱ� ���� ���
	enum class ComponentType {
		Transform,
		Collider,
		RigidBody,
		Script,
		Sprite,
		Animator,
		Camera,
		AudioListner,
		AudioSource,
		MaxComponentType
	};

	// Layer�� �������ϴ� ����
	enum class LayerType {
		None,
		Background,
		Tile,
		Actor,
		Player,
		Enemy,
		UI,
		MaxLayerType
	};

	enum class ResourceType {
		Texture,
		Sprite,
		Animation,
		AudioClip, // Sound
		Prefab, // ������ ����
		MaxResourceType
	};

	enum class ColliderType {
		Circle,
		Rectangle,
		Sphere,
		Box,
		Mesh,
		MaxColliderType
	};

	enum class UIType {
		HpBar,
		Button,
		MaxUIType
	};
}