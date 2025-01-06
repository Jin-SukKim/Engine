#pragma once
#include "Entity.h"

namespace JE {
	class Resource : public Entity 
	{
		using Super = Entity;
	public:
		// 무조건 type을 지정해 사용하도록 default 생성자 삭제
		Resource() = delete; 
		Resource(const std::wstring& name, ResourceType type = ResourceType::None) : Super(name), _type(type) {}
		virtual ~Resource() {}

		// 메모리에 asset을 load
		virtual bool Load(const std::wstring& path) = 0; 

	private:
		const ResourceType _type;
	};
}
