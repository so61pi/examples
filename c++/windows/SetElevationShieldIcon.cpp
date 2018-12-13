#include <Windows.h>
#include <CommCtrl.h>

#pragma comment(lib, "Comctl32.lib")

// enable visual style
#pragma comment(linker, "\"/manifestdependency:type='win32' \
    name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
    processorArchitecture='*' publicKeyToken='6595b64144ccf1df' \
    language='*'\"")


const WORD IDM_OK = 1;


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


int APIENTRY wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPTSTR lpCmdLine,
    _In_ int nCmdShow)
{
    const wchar_t szWindowClass[] = L"WindowClass";
    const wchar_t szWindowTitle[] = L"Title";

    // register class
    WNDCLASSEX wcex{};
    wcex.cbSize = sizeof(wcex);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = szWindowClass;
    if (!RegisterClassEx(&wcex)) {
        MessageBox(nullptr, L"Cannot register class.", L"Message", MB_ICONINFORMATION);
        return -1;
    }

    // create window
    auto hWnd = CreateWindow(szWindowClass, szWindowTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
    if (hWnd == nullptr) {
        MessageBox(nullptr, L"Cannot create window.", L"Message", MB_ICONINFORMATION);
        return -1;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // enter message loop
    MSG msg{};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE: {
        auto hButton = CreateWindow(
            L"BUTTON", L"OK",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            10, 10, 100, 50,
            hWnd,
            (HMENU)IDM_OK,
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            nullptr);
        if (hButton == nullptr)
            MessageBox(hWnd, L"Cannot create button.", L"Message", MB_ICONINFORMATION);
        else
            // can use SendMessage with BCM_SETSHIELD instead
            Button_SetElevationRequiredState(hButton, TRUE);

        break;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}
