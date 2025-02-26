#include "pch.h"
#include "UIManager.h"

namespace JE {
	void UIManager::Init()
	{
		// UI ��ü ����
		//std::unique_ptr<UIHUD> hud = std::make_unique<UIHUD>();
		//_uis[hud->GetUIType()] = std::move(hud);

		for (auto& [key, ui] : _uis)
			ui->Init();
	}

	void UIManager::OnLoad(UIType type)
	{
		// UI ã��
		std::unordered_map<UIType, std::unique_ptr<UIBase>>::iterator iter = _uis.find(type);

		// ����
		if (iter == _uis.end()) {
			OnFail();
			return;
		}

		// ����
		OnComplete(iter->second.get());
	}

	void UIManager::Tick(const float& DeltaTime)
	{
		// stack�� copy�� �����ͼ� loop ������
		std::stack<UIBase*> uiBases = _uiBases;
		while (!uiBases.empty()) {
			UIBase* uiBase = uiBases.top();
			uiBases.pop();
			if (uiBase)
				uiBase->Tick(DeltaTime);
		}

		// ���ο� UI�� �׷��޶�� �޽����� �Դٸ�
		if (_requestUIQueue.size() > 0) { // �� �����ӿ� �� ���� UI â�� ��������
			UIType requestUI = _requestUIQueue.front();
			_requestUIQueue.pop();
			OnLoad(requestUI);
		}
	}

	void UIManager::Render(IRenderer* r)
	{
		std::stack<UIBase*> uiBases = _uiBases;
		while (!uiBases.empty()) {
			UIBase* uiBase = uiBases.top();
			uiBases.pop();
			if (uiBase)
				uiBase->Render(r);
		}
	}

	void UIManager::OnComplete(UIBase* addUI)
	{
		if (addUI == nullptr)
			return;

		addUI->Init();
		addUI->Active();

		// ���࿡ ���� �߰��� ui�� ��üȭ���̶��
		// ��üȭ���� ui ���� �������� ���� ��Ȱ��ȭ
		if (addUI->IsFullScreen()) {
			std::stack<UIBase*> uiBases = _uiBases;
			while (!uiBases.empty()) {
				UIBase* uiBase = uiBases.top();
				uiBases.pop();
				if (uiBase)
					uiBase->InActive();
			}
		}

		// UI �߰�
		_uiBases.push(addUI);
		_activeUI = nullptr; // �� UI�� �߰��Ǿ����� ���� ������� UI�� nullptr
	}

	void UIManager::OnFail()
	{
		_activeUI = nullptr;
	}

	void UIManager::Open(UIType type)
	{
		_requestUIQueue.push(type); // �޽��� ť�� ��û �߰�
	}

	void UIManager::Close(UIType type)
	{
		if (_uiBases.size() <= 0)
			return;

		// �ش� UI ����
		std::stack<UIBase*> tempStack;
		UIBase* uiBase = nullptr;
		while (_uiBases.size() > 0) {
			uiBase = _uiBases.top();
			_uiBases.pop();

			if (uiBase->GetUIType() != type) {
				tempStack.push(uiBase);
				continue;
			}

			if (uiBase->IsFullScreen()) {
				std::stack<UIBase*> uiBases = _uiBases;
				while (!uiBases.empty()) {
					UIBase* uibase = uiBases.top();
					uiBases.pop();
					if (uibase) {
						uibase->Active();
						break;
					}
				}
			}

			uiBase->UIClear();
		}

		while (tempStack.size() > 0) {
			uiBase = tempStack.top();
			tempStack.pop();
			_uiBases.push(uiBase);
		}
	}
}