#include "pch.h"
#include "Animator2DComponent.h"
#include "SpriteActor.h"
#include "Mesh2DComponent.h"
#include "AssetManager.h"

namespace JE {
	void Animator2DComponent::Init()
	{
	}
	void Animator2DComponent::Tick(const float& DeltaTime)
	{
		if (_activeAnimation) {
			_activeAnimation->Tick(DeltaTime);

			// Active ANimation의 Events 가져오기
			Events* events = FindEvents(_activeAnimation->GetName());

			// Animation이 끝났으면
			if (_activeAnimation->IsComplete()) {
				if (events)
					events->CompleteEvent();
			}
		}
	}

	void Animator2DComponent::Render(IRenderer* r)
	{
		SpriteActor* owner = dynamic_cast<SpriteActor*>(this->GetOwner());
		if (!owner)
			return;

		Mesh2DComponent* mesh = owner->GetComponent<Mesh2DComponent>();
		if (!mesh)
			return;

		if (_activeAnimation) {
			// 현재 Sprite Animation의 frame이 AssetManager에 저장되어 있다면 가져와서 사용
			mesh->SetTexture(LoadSprite());
		}
	}

	const std::wstring& Animator2DComponent::CreateAnimation(const std::wstring& name, Texture* spriteSheet, Vector2 leftTop, Vector2 size, Vector2 offset, uint32 spriteLength, float duration)
	{
		Flipbook* anim = FindAnimation(name);
		// 이미 생성했었다면
		if (anim)
			return name;

		// 새로운 Animation 생성
		std::unique_ptr<Flipbook> newAnim = std::make_unique<Flipbook>();
		newAnim->SetName(name);
		newAnim->CreateAnimation(name, spriteSheet, leftTop, size, offset, spriteLength, duration);
		newAnim->SetAnimator(this);

		_animations[name] = std::move(newAnim);

		// Animation의 Event도 초기화
		std::unique_ptr<Events> newEvents = std::make_unique<Events>();
		_events[name] = std::move(newEvents);

		return name;
	}

	const std::wstring& Animator2DComponent::CreateAnimationByFolder(const std::wstring& name, const std::wstring& path, Vector2 offset, float duration)
	{
		Flipbook* anim = FindAnimation(name);
		if (anim)
			return name;

		int count = 0;
		fs::path fs(path);
		std::vector<Texture*> images = {};
		for (auto p : fs::recursive_directory_iterator(fs)) {
			std::wstring fullPath = p.path();
			std::wstring fileName = p.path().filename();

			// 경로의 이미지 가져오기
			Texture* texture = AssetManager::Load<Texture>(fileName, fullPath);
			images.emplace_back(texture);
			++count;
		}

		uint32 width = images[0]->GetWidth();
		uint32 height = images[0]->GetHeight();

		uint32 sheetWidth = width * count;
		Texture* spriteSheet = Texture::Create(name, sheetWidth, height); // 새로운 sprite sheet 생성

		// 생성한 sprite sheet에 sprite image들로 채우기
		std::vector<Color>& buffer = spriteSheet->GetBuffer();
		for (size_t i = 0; i < images.size(); ++i) {
			std::vector<Color>& sprite = images[i]->GetBuffer();
			for (uint32 y = 0; y < height; ++y)
				// x 한줄씩 copy
				std::copy(sprite.begin() + y * width, sprite.begin() + (y + 1) * width, buffer.begin() + (i * width) + y * sheetWidth);
				//for (uint32 x = 0; x < width; ++x) 
				//	buffer[(i * width + x) + y * sheetWidth] = sprite[x + y * width];
		}

		CreateAnimation(name, spriteSheet, Vector2::Zero, Vector2(static_cast<float>(width), static_cast<float>(height)), offset, count, duration);
		return name;
	}

	Flipbook* Animator2DComponent::FindAnimation(const std::wstring& name)
	{
		auto iter = _animations.find(name);
		if (iter == _animations.end())
			return nullptr;

		return iter->second.get();
	}

	void Animator2DComponent::PlayAnimation(const std::wstring& name)
	{
		Flipbook* anim = FindAnimation(name);
		if (!anim)
			return;

		// 이전 Animation
		if (_activeAnimation) {
			// 바뀔 Animation의 End Event
			Events* events = FindEvents(name);
			if (events)
				events->EndEvent();
		}

		// 초기화
		_activeAnimation = anim;
		_activeAnimation->Reset();

		// 새로운 Animation의 Start Event
		Events* events = FindEvents(name);
		if (events)
			events->StartEvent();
	}

	Animator2DComponent::Events* Animator2DComponent::FindEvents(const std::wstring& name)
	{
		auto iter = _events.find(name);
		if (iter == _events.end())
			return nullptr;

		return iter->second.get();
	}

	void Animator2DComponent::SetEvents(const std::wstring& name, std::function<void()> start, std::function<void()> complete, std::function<void()> end)
	{
		Events* e = FindEvents(name);
	
		if (start)
			e->StartEvent = start;
		if (complete)
			e->CompleteEvent = complete;
		if (end)
			e->EndEvent = end;
	}

	Animator2DComponent::Event* Animator2DComponent::GetStartEvent(const std::wstring& name)
	{
		Events* events = FindEvents(name);
		if (!events)
			return nullptr;
		return &events->StartEvent;
	}

	Animator2DComponent::Event* Animator2DComponent::GetCompleteEvent(const std::wstring& name)
	{
		Events* events = FindEvents(name);
		if (!events)
			return nullptr;
		return &events->CompleteEvent;
	}

	Animator2DComponent::Event* Animator2DComponent::GetEndEvent(const std::wstring& name)
	{
		Events* events = FindEvents(name);
		if (!events)
			return nullptr;
		return &events->EndEvent;
	}

	const std::wstring& Animator2DComponent::LoadSprite()
	{
		// Sprite Sheet에서 그릴 부분만 sub texture에 복사해 렌더링 준비
		_frameKey = std::format(L"{0}_{1}", _activeAnimation->GetName(), _activeAnimation->GetIndex());
		
		Texture* sprite = AssetManager::Find<Texture>(_frameKey);
		if (sprite)
			return _frameKey;
			
		sprite = Texture::Create(_frameKey, static_cast<uint32>(_activeAnimation->GetSize().X), static_cast<uint32>(_activeAnimation->GetSize().Y));

		std::vector<Color>& buffer = sprite->GetBuffer();
		_activeAnimation->SetSpriteBuffer(buffer);

		return _frameKey;
	}
}