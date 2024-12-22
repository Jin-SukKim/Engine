#pragma once

namespace JE {
class Object
{
public:
	Object() {}
	virtual ~Object() {}

	void SetName(const std::wstring& name) { _name = name; }
	const std::wstring& GetName() { return _name; }

private:
	std::wstring _name = L"";
};
};
