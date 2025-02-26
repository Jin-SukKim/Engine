#pragma once
#include "UIBase.h"

namespace JE {
	class UIManager
	{
	public:
		void Init();
		void OnLoad(UIType type); // UI를 호출했을 때
		void Tick(const float& DeltaTime);
		void Render(IRenderer* r);
		void OnComplete(UIBase* addUI); // UI를 닫을 때
		void OnFail(); // UI 열기에 실패했을때

		// 새 UI 추가
		template<typename T>
		T* Push(UIType type);

		void Open(UIType type); // UI 창 띄우기 (메시지 큐에 요청 추가)
		void Close(UIType type); // UI 창 닫기
	private:
		std::unordered_map<UIType, std::unique_ptr<UIBase>> _uis = {};
		std::stack<UIBase*> _uiBases = {}; // ui는 나중에 열리는 창이 가장 위에 있고 가장 먼저 열린 창을 마지막으 닫음
		std::queue<UIType> _requestUIQueue = {}; // 메시지 큐 (순서가 중요한 기능에서 사용)
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
