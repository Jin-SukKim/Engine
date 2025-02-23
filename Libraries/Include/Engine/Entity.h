#pragma once

namespace JE {
	class IRenderer;
	class Entity
	{
	public:
		Entity() {}
		Entity(const std::wstring& name) : _name(name) {}
		virtual ~Entity() {}

		virtual void Init() {};
		virtual void Tick(const float& DeltaTime) {};
		virtual void Render(IRenderer* r) {};

		void SetName(const std::wstring& name) { _name = name; }
		const std::wstring& GetName() { return _name; }
	private:
		std::wstring _name = L"";
	};
}

