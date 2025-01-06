#pragma once

namespace JE {
	// 각 오브젝트가 가지고 있는 Component의 Update 순서를 지정하기 위해 사용
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

	// Layer를 렌더링하는 순서
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
		Prefab, // 데이터 저장
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