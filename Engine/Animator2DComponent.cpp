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

			// Active ANimation�� Events ��������
			Events* events = FindEvents(_activeAnimation->GetName());

			// Animation�� ��������
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
			// ���� Sprite Animation�� frame�� AssetManager�� ����Ǿ� �ִٸ� �����ͼ� ���
			mesh->SetTexture(LoadSprite());
		}
	}

	const std::wstring& Animator2DComponent::CreateAnimation(const std::wstring& name, Texture* spriteSheet, Vector2 leftTop, Vector2 size, Vector2 offset, uint32 spriteLength, float duration)
	{
		Flipbook* anim = FindAnimation(name);
		// �̹� �����߾��ٸ�
		if (anim)
			return name;

		// ���ο� Animation ����
		std::unique_ptr<Flipbook> newAnim = std::make_unique<Flipbook>();
		newAnim->SetName(name);
		newAnim->CreateAnimation(name, spriteSheet, leftTop, size, offset, spriteLength, duration);
		newAnim->SetAnimator(this);

		_animations[name] = std::move(newAnim);

		// Animation�� Event�� �ʱ�ȭ
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

			// ����� �̹��� ��������
			Texture* texture = AssetManager::Load<Texture>(fileName, fullPath);
			images.emplace_back(texture);
			++count;
		}

		uint32 width = images[0]->GetWidth();
		uint32 height = images[0]->GetHeight();

		uint32 sheetWidth = width * count;
		Texture* spriteSheet = Texture::Create(name, sheetWidth, height); // ���ο� sprite sheet ����

		// ������ sprite sheet�� sprite image��� ä���
		std::vector<Color>& buffer = spriteSheet->GetBuffer();
		for (size_t i = 0; i < images.size(); ++i) {
			std::vector<Color>& sprite = images[i]->GetBuffer();
			for (uint32 y = 0; y < height; ++y)
				// x ���پ� copy
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

		// ���� Animation
		if (_activeAnimation) {
			// �ٲ� Animation�� End Event
			Events* events = FindEvents(name);
			if (events)
				events->EndEvent();
		}

		// �ʱ�ȭ
		_activeAnimation = anim;
		_activeAnimation->Reset();

		// ���ο� Animation�� Start Event
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
		// Sprite Sheet���� �׸� �κи� sub texture�� ������ ������ �غ�
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