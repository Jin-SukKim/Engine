#pragma once
#include "Object.h"

namespace JE {
	class Actor : public Object
	{
		using Super = Object;
	public:
		Actor(const std::wstring& name);
		virtual ~Actor() override {}
		
		virtual void Init();
		virtual void Tick(const float& DeltaTime);
		virtual void Render(IRenderer* r);

	private:
		// TransformComponent Ãß°¡
		void initTransform();
	};
}

