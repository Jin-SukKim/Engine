#pragma once
#include "WindowsApp.h"

namespace JE {
    class MainEditor : public WindowsApp
    {
        using Super = WindowsApp;
    public:
        MainEditor(const HINSTANCE& hInstance, const std::wstring& windowName, Dimension dimension);
        virtual ~MainEditor() override;

        bool Init(int nCmdShow) override;
        void Render() override;

        void LoadScenes() override;

        virtual LRESULT CALLBACK Proc(HWND hwnd, UINT32 msg, WPARAM wParam, LPARAM lParam);
    private:
    };
}
