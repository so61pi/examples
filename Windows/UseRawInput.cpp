#include <exception>
#include <sstream>
#include <stdexcept>
#include <vector>

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

    static std::wstringstream ss;

    /*
    Generic Desktop Page (0x01)
        Usage ID
            - Mouse (0x02)
            - Keyboard (0x06)

    For more information
        http://www.usb.org/developers/hidpage/Hut1_12v2.pdf
    */

    switch (message) {
    case WM_CREATE: {
        RAWINPUTDEVICE rid{};

        // add HID keyboard and also ignore legacy keyboard messages
        rid.usUsagePage = 0x01;
        rid.usUsage = 0x06;
        rid.dwFlags = RIDEV_NOLEGACY;
        rid.hwndTarget = nullptr;

        // register for the raw input the device
        if (RegisterRawInputDevices(&rid, 1, sizeof(rid)) == FALSE)
            MessageBox(hWnd, L"Cannot register raw input device.", L"Error", MB_ICONERROR);
        
        break;
    }

    case WM_INPUT: {
        try {
            UINT rawDataSize = 0;

            // get input data size
            if (GetRawInputData(
                reinterpret_cast<HRAWINPUT>(lParam),
                RID_INPUT,
                nullptr,
                &rawDataSize,
                sizeof(RAWINPUTHEADER)) != 0)
            {
                throw std::runtime_error("Cannot get raw input data.");
            }

            std::vector<BYTE> rawData(rawDataSize);

            // get raw input data
            if (GetRawInputData(
                reinterpret_cast<HRAWINPUT>(lParam),
                RID_INPUT,
                rawData.data(),
                &rawDataSize,
                sizeof(RAWINPUTHEADER)) == -1)
            {
                throw std::runtime_error("Cannot get raw input data.");
            }

            // get the key
            RAWINPUT& rawInput = *reinterpret_cast<RAWINPUT *>(rawData.data());
            if (rawInput.header.dwType == RIM_TYPEKEYBOARD) {
                ss << L"Keyboard: "
                    << L"MakeCode:" << rawInput.data.keyboard.MakeCode << L' '
                    << L"VKey:" << rawInput.data.keyboard.VKey << L' '
                    << L"Flags:" << rawInput.data.keyboard.Flags << L'\n';
            }
        }
        catch (const std::exception& e) {
            MessageBoxA(hWnd, e.what(), "Error", MB_ICONERROR);
        }
        catch (...) {
            MessageBox(hWnd, L"Unknown exception", L"Error", MB_ICONERROR);
        }
        break;
    }

    case WM_DESTROY:
        // put an end on it
        ss << L'\0';

        // then output to the debugger
        OutputDebugString(ss.str().c_str());

        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}
