#pragma once

#include <windows.h>
#include <iostream>

class WindowManager {
public:
    WindowManager(HINSTANCE hInstance, LPCWSTR title, LPCWSTR className, int nCmdShow);

    bool Init();
    void RunMessageLoop();

private:
    HINSTANCE m_hInstance;
    LPCWSTR m_title;
    LPCWSTR m_className;
    int m_nCmdShow;
    HWND m_hwnd;

    HWND m_btnStartGame;
    HWND m_btnOptions;
    HWND m_btnExit;

    HWND m_staticText;

    HWND CreateStaticText(LPCWSTR text, int width, int height, int yOffset, int fontSize);
    HWND CreateButton(LPCWSTR text, int width, int height, int yOffset);
    LRESULT ButtonProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    POINT CalculateControlPosition(int width, int yOffset);
};