#include "pch.h"
#include "UIManager.h"

namespace JE {
	void UIManager::Init()
	{
		// UI 객체 생성
		//std::unique_ptr<UIHUD> hud = std::make_unique<UIHUD>();
		//_uis[hud->GetUIType()] = std::move(hud);

		for (auto& [key, ui] : _uis)
			ui->Init();
	}

	void UIManager::OnLoad(UIType type)
	{
		// UI 찾기
		std::unordered_map<UIType, std::unique_ptr<UIBase>>::iterator iter = _uis.find(type);

		// 실패
		if (iter == _uis.end()) {
			OnFail();
			return;
		}

		// 성공
		OnComplete(iter->second.get());
	}

	void UIManager::Tick(const float& DeltaTime)
	{
		// stack은 copy로 가져와서 loop 돌리기
		std::stack<UIBase*> uiBases = _uiBases;
		while (!uiBases.empty()) {
			UIBase* uiBase = uiBases.top();
			uiBases.pop();
			if (uiBase)
				uiBase->Tick(DeltaTime);
		}

		// 새로운 UI를 그려달라는 메시지가 왔다면
		if (_requestUIQueue.size() > 0) { // 한 프레임에 한 개의 UI 창만 열리도록
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

		// 만약에 현재 추가된 ui가 전체화면이라면
		// 전체화면인 ui 말고 나머지를 전부 비활성화
		if (addUI->IsFullScreen()) {
			std::stack<UIBase*> uiBases = _uiBases;
			while (!uiBases.empty()) {
				UIBase* uiBase = uiBases.top();
				uiBases.pop();
				if (uiBase)
					uiBase->InActive();
			}
		}

		// UI 추가
		_uiBases.push(addUI);
		_activeUI = nullptr; // 새 UI가 추가되었으니 현재 사용중인 UI는 nullptr
	}

	void UIManager::OnFail()
	{
		_activeUI = nullptr;
	}

	void UIManager::Open(UIType type)
	{
		_requestUIQueue.push(type); // 메시지 큐에 요청 추가
	}

	void UIManager::Close(UIType type)
	{
		if (_uiBases.size() <= 0)
			return;

		// 해당 UI 제거
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