#pragma once
#include "Entity.h"

namespace JE {
	class Resource : public Entity 
	{
		using Super = Entity;
	public:
		// ������ type�� ������ ����ϵ��� default ������ ����
		Resource() = delete; 
		Resource(const std::wstring& name, ResourceType type = ResourceType::None) : Super(name), _type(type) {}
		virtual ~Resource() {}

		// �޸𸮿� asset�� load
		virtual bool Load(const std::wstring& path) = 0; 

	private:
		const ResourceType _type;
	};
}
