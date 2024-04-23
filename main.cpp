#include <windows.h>
#include "WindowManager.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
    WindowManager window(hInstance, L"Snake Game", L"SnakeGameWindowClass", nCmdShow);
    if (!window.Init()) {
        return 1;
    }
    window.RunMessageLoop();
    return 0;
}