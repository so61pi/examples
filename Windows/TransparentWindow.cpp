#include <Windows.h>
#include <CommCtrl.h>

#pragma comment(lib, "Comctl32.lib")

// enable visual style
#pragma comment(linker, "\"/manifestdependency:type='win32' \
    name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
    processorArchitecture='*' publicKeyToken='6595b64144ccf1df' \
    language='*'\"")


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
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = szWindowClass;
    if (!RegisterClassEx(&wcex)) {
        MessageBox(NULL, L"Cannot register class.", L"Message", MB_ICONINFORMATION);
        return -1;
    }

    // create window
    auto hWnd = CreateWindow(szWindowClass, szWindowTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
    if (hWnd == NULL) {
        MessageBox(NULL, L"Cannot create window.", L"Message", MB_ICONINFORMATION);
        return -1;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // enter message loop
    MSG msg{};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE: {
        SetWindowLongPtr(hWnd,
            GWL_EXSTYLE,
            GetWindowLongPtr(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
        if (SetLayeredWindowAttributes(hWnd, 0, 200, LWA_ALPHA) == FALSE) {
            MessageBox(hWnd,
                L"SetLayeredWindowAttributes failed!",
                L"Message",
                MB_ICONWARNING);
        }
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
