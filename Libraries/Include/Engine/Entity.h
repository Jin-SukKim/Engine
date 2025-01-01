#pragma once

namespace JE {
	class Entity
	{
	public:
		Entity() {}
		Entity(const std::wstring& name) : _name(name) {}
		virtual ~Entity() {}

		virtual void Init() = 0;
		virtual void Tick(const float& DeltaTime) = 0;
		virtual void Render() = 0;

		void SetName(const std::wstring& name) { _name = name; }
		const std::wstring& GetName() { return _name; }
	private:
		std::wstring _name = L"";
	};
}

