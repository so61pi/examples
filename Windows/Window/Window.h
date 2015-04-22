#ifndef WINDOW_H
#define WINDOW_H


#include <functional>
#include <map>
#include <string>
#include <utility>

#include <Windows.h>


class Window {
public:
    using MESSAGEHANDLER = std::function<LRESULT(Window *const, UINT, WPARAM, LPARAM, LPVOID)>;

    // start message loop
    WPARAM DoMessageLoop();

    void AddMessageHandler(UINT message, LPVOID param, MESSAGEHANDLER handler);
    void RemoveMessageHandler(UINT message);

    // get associated window handle
    HWND GetHWND();

    HWND Create(
        const std::wstring& windowClassName,
        const std::wstring& windowTitle,
        DWORD dwStyle = WS_OVERLAPPEDWINDOW,
        int x = CW_USEDEFAULT,
        int y = 0,
        int width = CW_USEDEFAULT,
        int height = 0,
        HWND hWndParent = nullptr,
        HMENU hMenu = nullptr);

    void Show(int flags);

private:
    void Register(const std::wstring& windowClassName);

private:
    // window message handler
    LRESULT CALLBACK WndProc(UINT message, WPARAM wParam, LPARAM lParam);

private:
    // receive window message from all instances of Window class
    // then dispatch the message to appropriate Window
    static LRESULT CALLBACK WndProcAll(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private: // data
    HWND m_hWnd = nullptr;
    std::map<UINT, std::pair<MESSAGEHANDLER, LPVOID>> m_messageHandlers;
};


#endif // WINDOW_H
