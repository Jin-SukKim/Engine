#pragma once
#include "UIBase.h"

namespace JE {
	class UIManager
	{
	public:
		void Init();
		void OnLoad(UIType type); // UI�� ȣ������ ��
		void Tick(const float& DeltaTime);
		void Render(IRenderer* r);
		void OnComplete(UIBase* addUI); // UI�� ���� ��
		void OnFail(); // UI ���⿡ ����������

		// �� UI �߰�
		template<typename T>
		T* Push(UIType type);

		void Open(UIType type); // UI â ���� (�޽��� ť�� ��û �߰�)
		void Close(UIType type); // UI â �ݱ�
	private:
		std::unordered_map<UIType, std::unique_ptr<UIBase>> _uis = {};
		std::stack<UIBase*> _uiBases = {}; // ui�� ���߿� ������ â�� ���� ���� �ְ� ���� ���� ���� â�� �������� ����
		std::queue<UIType> _requestUIQueue = {}; // �޽��� ť (������ �߿��� ��ɿ��� ���)
		UIBase* _activeUI = nullptr;
	};

	template<typename T>
	inline T* UIManager::Push(UIType type)
	{
		auto iter = _uis.find(type);
		if (iter == _uis.end())
			return nullptr;

		std::unique_ptr<T> ui = std::make_unique<T>();
		_uis[ui->GetUIType()] = std::move(ui);

		return _uis[ui->GetUIType()].get();
	}
}
