#include <exception>
#include <functional>

#include <Windows.h>
#include "Window.h"


int APIENTRY wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPTSTR lpCmdLine,
    _In_ int nCmdShow)
{
    try {
        wchar_t message[] = L"Hello";
        Window window;

        auto createHandler =
            [](Window *const window, UINT, WPARAM, LPARAM, LPVOID param) -> LRESULT {
                auto message = reinterpret_cast<wchar_t *>(param);
                MessageBox(window->GetHWND(), message, L"Title", MB_ICONINFORMATION);
                return 0;
            };

        // add message handler for WM_CREATE and WM_DESTROY
        window.AddMessageHandler(
            WM_CREATE,
            std::bind(createHandler,
                std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3, std::placeholders::_4,
                reinterpret_cast<LPVOID>(message)));
        window.AddMessageHandler(
            WM_DESTROY,
            [](Window *const window, UINT, WPARAM, LPARAM) -> LRESULT {
                MessageBox(window->GetHWND(), L"Goodbye", L"Title", MB_ICONINFORMATION);
                PostQuitMessage(0);
                return 0;
            });

        // now create window
        window.Create(L"WindowClass", L"Title");
        window.Show(nCmdShow);

        // and enter message loop
        return (int)window.DoMessageLoop();
    }
    catch (const std::exception& e) {
        MessageBoxA(nullptr, e.what(), "Error", MB_ICONERROR);
    }
    catch (...) {
        MessageBox(nullptr, L"Unknown exception.", L"Error", MB_ICONERROR);
    }
    return -1;
}
