#include "Window.h"

#include <stdexcept>

#include <Windows.h>
#include <CommCtrl.h>

#pragma comment(lib, "Comctl32.lib")

// enable visual style
#pragma comment(linker, "\"/manifestdependency:type='win32' \
    name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
    processorArchitecture='*' publicKeyToken='6595b64144ccf1df' \
    language='*'\"")


LRESULT CALLBACK Window::WndProcAll(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    if (message == WM_NCCREATE) {
        auto pCreateStruct = reinterpret_cast<CREATESTRUCT *>(lParam);
        auto self = reinterpret_cast<Window *>(pCreateStruct->lpCreateParams);
        self->m_hWnd = hWnd;
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(self));
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    else {
        auto window = reinterpret_cast<Window *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        if (window)
            return window->WndProc(message, wParam, lParam);
        else
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
}


LRESULT CALLBACK Window::WndProc(
    UINT message,
    WPARAM wParam,
    LPARAM lParam)
{
    try {
        std::lock_guard<mutex_type> lg{ m_mutex };

        if (m_messageHandlers.find(message) != m_messageHandlers.end()) {
            auto handler = m_messageHandlers[message];
            return handler(this, message, wParam, lParam);
        }
        else if (message == WM_DESTROY) {
            PostQuitMessage(0);
            return 0;
        }
        else
            return DefWindowProc(m_hWnd, message, wParam, lParam);
    }
    catch (...) {
        return DefWindowProc(m_hWnd, message, wParam, lParam);
    }
}


HWND Window::GetHWND() const {
    return m_hWnd;
}


void Window::Register(const std::wstring& windowClassName) const {
    // register class
    WNDCLASSEX wcex{};
    wcex.cbSize = sizeof(wcex);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = &Window::WndProcAll;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = GetModuleHandle(nullptr);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
    wcex.lpszClassName = windowClassName.c_str();
    if (!RegisterClassEx(&wcex))
        throw std::runtime_error("Cannot register class.");
}


HWND Window::Create(
    const std::wstring& windowClassName,
    const std::wstring& windowTitle,
    DWORD dwStyle,
    int x,
    int y,
    int width,
    int height,
    HWND hWndParent,
    HMENU hMenu)
{
    Register(windowClassName);

    // create window
    m_hWnd = CreateWindow(
        windowClassName.c_str(),
        windowTitle.c_str(),
        dwStyle,
        x, y,
        width, height,
        hWndParent,
        hMenu,
        GetModuleHandle(nullptr),
        reinterpret_cast<LPVOID>(this));
    if (m_hWnd == nullptr)
        throw std::runtime_error("Cannot create window.");

    return m_hWnd;
}


void Window::Show(int flags) const {
    ShowWindow(m_hWnd, flags);
    UpdateWindow(m_hWnd);
}


WPARAM Window::DoMessageLoop() const {
    // enter message loop
    MSG msg{};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}


void Window::AddMessageHandler(UINT message, MESSAGEHANDLER handler) {
    std::lock_guard<mutex_type> lg{ m_mutex };

    if (handler == nullptr)
        throw std::invalid_argument("Message handler cannot be null.");

    m_messageHandlers[message] = handler;
}


void Window::RemoveMessageHandler(UINT message) {
    std::lock_guard<mutex_type> lg{ m_mutex };

    m_messageHandlers.erase(message);
}

