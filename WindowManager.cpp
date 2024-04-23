#include "WindowManager.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

WindowManager::WindowManager(HINSTANCE hInstance, LPCWSTR title, LPCWSTR className, int nCmdShow)
    : m_hInstance(hInstance), m_title(title), m_className(className), m_nCmdShow(nCmdShow),
    m_hwnd(nullptr), m_btnStartGame(nullptr), m_btnOptions(nullptr), m_btnExit(nullptr), m_staticText(nullptr) {}

bool WindowManager::Init() {
    WNDCLASS wc = { };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = m_hInstance;
    wc.lpszClassName = m_className;
    if (!RegisterClass(&wc)) {
        std::cerr << "Failed to register window class." << std::endl;
        return false;
    }

    m_hwnd = CreateWindowEx(
        0,
        m_className,
        m_title,
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
        (GetSystemMetrics(SM_CXSCREEN) - 800) / 2,
        (GetSystemMetrics(SM_CYSCREEN) - 600) / 2,
        800,
        600,
        NULL,
        NULL,
        m_hInstance,
        NULL
    );

    if (m_hwnd == nullptr) {
        std::cerr << "Failed to create window." << std::endl;
        UnregisterClass(m_className, m_hInstance);
        return false;
    }

    m_staticText = CreateStaticText(L"Snake Game", 200, 50, 50, 25);

    m_btnStartGame = CreateButton(L"Start Game", 200, 50, 150);
    m_btnOptions = CreateButton(L"Options", 200, 50, 230);
    m_btnExit = CreateButton(L"Exit", 200, 50, 310);

    ShowWindow(m_hwnd, m_nCmdShow);

    return true;
}

void WindowManager::RunMessageLoop() {
    MSG msg = { };
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

HWND WindowManager::CreateStaticText(LPCWSTR text, int width, int height, int yOffset, int fontSize) {
    POINT position = CalculateControlPosition(width, yOffset);
    int x = position.x;
    int y = position.y;

    HWND hwndStatic = CreateWindow(
        L"STATIC",
        text,
        WS_VISIBLE | WS_CHILD | SS_CENTER,
        x,
        y,
        width,
        height,
        m_hwnd,
        NULL,
        m_hInstance,
        NULL
    );

    HFONT hFont = CreateFont(fontSize, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");
    if (hFont != NULL) {
        SendMessage(hwndStatic, WM_SETFONT, (WPARAM)hFont, TRUE);
    }

    return hwndStatic;
}


HWND WindowManager::CreateButton(LPCWSTR text, int width, int height, int yOffset) {
    POINT position = CalculateControlPosition(width, yOffset);
    int x = position.x;
    int y = position.y;

    return CreateWindow(
        L"BUTTON",
        text,
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        x,
        y,
        width,
        height,
        m_hwnd,
        NULL,
        m_hInstance,
        NULL
    );
}


LRESULT WindowManager::ButtonProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_COMMAND:
        if (HIWORD(wParam) == BN_CLICKED) {
            if (hwnd == m_btnStartGame) {
                MessageBox(NULL, L"Starting game...", L"Start Game", MB_OK);
            }
            else if (hwnd == m_btnOptions) {
                MessageBox(NULL, L"Opening options...", L"Options", MB_OK);
            }
            else if (hwnd == m_btnExit) {
                PostQuitMessage(0);
            }
        }
        break;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

POINT WindowManager::CalculateControlPosition(int width, int yOffset) {
    RECT clientRect;
    GetClientRect(m_hwnd, &clientRect);
    int windowWidth = clientRect.right - clientRect.left;

    int x = (windowWidth - width) / 2;
    int y = yOffset;

    POINT position = { x, y };
    return position;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CTLCOLORSTATIC: {
        HDC hdcStatic = (HDC)wParam;
        SetTextColor(hdcStatic, RGB(0, 0, 0));
        SetBkMode(hdcStatic, TRANSPARENT);
        return (LRESULT)GetStockObject(NULL_BRUSH);
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        if (hdc == nullptr) {
            std::cerr << "Failed to begin painting." << std::endl;
            return 0;
        }
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
        EndPaint(hwnd, &ps);
    }
                 return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}