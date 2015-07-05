#include <exception>
#include <functional>
#include <memory>
#include <stdexcept>
#include <boost/scope_exit.hpp>

#include <Windows.h>

#include "Window.h"
#include "ToastDemo.h"


WORD const IDM_BTN_CREATE_SHORTCUT = 1;
WORD const IDM_BTN_REMOVE_SHORTCUT = 2;
WORD const IDM_BTN_DISPLAY_TOAST = 3;


int APIENTRY wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPTSTR lpCmdLine,
    _In_ int nCmdShow)
{
    try {
        if (FAILED(CoInitialize(nullptr)))
            throw std::runtime_error("cannot initialize COM");

        BOOST_SCOPE_EXIT(void) {
            CoUninitialize();
        } BOOST_SCOPE_EXIT_END;


        // create 3 buttons, one for creating shortcut, one for removing shortcut,
        // and one for display toast
        auto createHandler = [](Window *const window, std::unique_ptr<ToastDemo>& toastDemo) -> LRESULT {
            try {
                auto CreateButton = [](HWND const hWnd, WORD const IDM, std::wstring const& text, int x, int y, int width, int height) -> HWND {
                    return CreateWindow(
                        L"BUTTON", text.c_str(),
                        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                        x, y, width, height,
                        hWnd,
                        reinterpret_cast<HMENU>(IDM),
                        reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hWnd, GWLP_HINSTANCE)),
                        nullptr
                    );
                };

                CreateButton(window->GetHWND(), IDM_BTN_CREATE_SHORTCUT, L"Create shortcut", 10, 10, 150, 30);
                CreateButton(window->GetHWND(), IDM_BTN_DISPLAY_TOAST, L"Display toast", 170, 10, 150, 30);
                CreateButton(window->GetHWND(), IDM_BTN_REMOVE_SHORTCUT, L"Remove shortcut", 340, 10, 150, 30);

                toastDemo = std::make_unique<ToastDemo>(window->GetHWND());
            }
            catch (std::exception const& e) {
                MessageBoxA(window->GetHWND(), e.what(), "Error", MB_ICONERROR);
            }

            return 0;
        };


        auto commandHandler = [](Window *const window, WPARAM wParam, std::unique_ptr<ToastDemo>& toastDemo) -> LRESULT {
            try {
                switch (LOWORD(wParam)) {
                case IDM_BTN_CREATE_SHORTCUT:
                    toastDemo->CreateShortcut();
                    MessageBox(window->GetHWND(), L"Shortcut is created.", L"Information", MB_ICONINFORMATION);
                    break;

                case IDM_BTN_REMOVE_SHORTCUT:
                    toastDemo->RemoveShortcut();
                    MessageBox(window->GetHWND(), L"Shortcut is removed.", L"Information", MB_ICONINFORMATION);
                    break;

                case IDM_BTN_DISPLAY_TOAST:
                    toastDemo->Display();
                    break;

                default:
                    break;
                }
            }
            catch (std::exception const& e) {
                MessageBoxA(window->GetHWND(), e.what(), "Error", MB_ICONERROR);
            }

            return 0;
        };


        auto destroyHandler = [](std::unique_ptr<ToastDemo>& toastDemo) -> LRESULT {
            toastDemo.release();
            PostQuitMessage(0);
            return 0;
        };


        Window window;
        std::unique_ptr<ToastDemo> toastDemo;

        // add message handlers
        window.AddMessageHandler(
            WM_CREATE,
            std::bind(createHandler,
                std::placeholders::_1,
                std::ref(toastDemo)
            )
        );
        window.AddMessageHandler(
            WM_COMMAND,
            std::bind(commandHandler,
                std::placeholders::_1,
                std::placeholders::_3,
                std::ref(toastDemo)
            )
        );
        window.AddMessageHandler(
            WM_DESTROY,
            std::bind(destroyHandler,
                std::ref(toastDemo)
            )
        );
        
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
