#include <Windows.h>


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

    // create accelerator table   
    ACCEL accel{};
    // Alt + O
    accel.fVirt = FALT | FVIRTKEY;
    accel.key = 'O';
    accel.cmd = IDM_OK;
    auto hAccel = CreateAcceleratorTable(&accel, 1);
    if (hAccel == NULL) {
        MessageBox(NULL, L"Cannot create accelerator table.", L"Message", MB_ICONINFORMATION);
    }

    // enter message loop
    MSG msg{};
    while (GetMessage(&msg, NULL, 0, 0)) {
        if (!TranslateAccelerator(msg.hwnd, hAccel, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE: {
        HWND hButton = CreateWindow(
            L"BUTTON", L"OK",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            10, 10, 100, 50,
            hWnd,
            (HMENU)IDM_OK,
            (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
            NULL);
        if (hButton == NULL)
            MessageBox(hWnd, L"Cannot create button.", L"Message", MB_ICONINFORMATION);
        break;
    }

    case WM_COMMAND:
        if (LOWORD(wParam) == IDM_OK) {
            switch (HIWORD(wParam)) {
            case 0:
                MessageBox(hWnd, L"OK is pressed.", L"Message", MB_ICONINFORMATION);
                break;

            case 1:
                MessageBox(hWnd, L"OK is pressed using accelerator.", L"Message", MB_ICONINFORMATION);
                break;
            }
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
