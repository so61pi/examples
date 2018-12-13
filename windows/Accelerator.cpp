#include <Windows.h>


const WORD IDM_CTRL_A = 1;
const WORD IDM_ATL_P = 2;


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

    // create accelerator table   
    ACCEL accel[2]{};
    // Ctrl + A
    accel[0].fVirt = FCONTROL | FVIRTKEY;
    accel[0].key = 'A';
    accel[0].cmd = IDM_CTRL_A;
    // Alt + P
    accel[1].fVirt = FALT | FVIRTKEY;
    accel[1].key = 'P';
    accel[1].cmd = IDM_ATL_P;
    auto hAccel = CreateAcceleratorTable(accel, 2);
    if (hAccel == nullptr) {
        MessageBox(nullptr, L"Cannot create accelerator table.", L"Message", MB_ICONINFORMATION);
    }

    // enter message loop
    MSG msg{};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        if (!TranslateAccelerator(msg.hwnd, hAccel, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE:
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDM_CTRL_A:
            MessageBox(hWnd, L"Ctrl+A is pressed.", L"Message", MB_ICONINFORMATION);
            break;

        case IDM_ATL_P:
            MessageBox(hWnd, L"Alt+P is pressed.", L"Message", MB_ICONINFORMATION);
            break;
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}
