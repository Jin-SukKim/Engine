#pragma once

namespace JE {
	// �� ������Ʈ�� ������ �ִ� Component�� Update ������ �����ϱ� ���� ���
	enum class ComponentType {
		Transform,
		Collider,
		RigidBody,
		Script,
		Mesh,
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
		None,
		Texture,
		Sprite,
		Mesh,
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
		Mesh2D,
		Mesh,
		MaxColliderType
	};

	enum class UIType {
		HpBar,
		Button,
		MaxUIType
	};
}