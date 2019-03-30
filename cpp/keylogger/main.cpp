#include <chrono>
#include <ctime>
#include <exception>
#include <functional>
#include <iomanip>
#include <memory>
#include <sstream>
#include <vector>

#include <Windows.h>
#include <TlHelp32.h>
#include <strsafe.h>
#include "Window.h"


class LogRecord {
public:
    //
    LogRecord(HWND hWnd) :
        m_hWnd(hWnd)
    {
        // get title of the given window
        m_windowTitle = GetWindowTitle();

        // get exe file name of the given window handle
        m_exeFileName = GetExeFileName();
    }


    //
    void Append(const std::wstring& keyName) {
        m_lastAppendTime = std::chrono::steady_clock::now();
        m_record += keyName;
    }


    //
    void Append(WCHAR c) {
        m_lastAppendTime = std::chrono::steady_clock::now();
        m_record.push_back(c);
    }


    //
    std::wstring GetRecordAsText() const {
        auto time = std::chrono::steady_clock::to_time_t(m_lastAppendTime);
        std::tm localTime{};
        localtime_s(&localTime, &time);

        std::wstringstream ss;
        ss 
            << L"[" << m_windowTitle << L"][" << std::put_time(&localTime, L"%Y-%m-%d %H:%M:%S") << L"]\n"
            << L"[" << m_exeFileName << L"]\n"
            << m_record;
        return ss.str();
    }


    //
    HWND GetHWND() const {
        return m_hWnd;
    }


    //
    std::chrono::steady_clock::time_point GetLastAppendTime() const {
        return m_lastAppendTime;
    }


private:
    //
    std::wstring GetWindowTitle() const {
        std::vector<WCHAR> windowTitle;

        // get window title length, including null character
        auto length = GetWindowTextLength(m_hWnd) + 1;
        windowTitle.resize(length);
        GetWindowText(m_hWnd, windowTitle.data(), windowTitle.size());
        windowTitle.pop_back(); // remove null character

        return std::wstring(windowTitle.begin(), windowTitle.end());
    }


    //
    std::wstring GetExeFileName() const {
        DWORD processId = 0;
        if (GetWindowThreadProcessId(m_hWnd, &processId) == 0)
            return L"";

        std::wstring exeFileName;
        auto hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (hSnapshot != INVALID_HANDLE_VALUE) {
            PROCESSENTRY32 pe{};
            pe.dwSize = sizeof(pe);
            if (Process32First(hSnapshot, &pe) != FALSE) {
                do {
                    if (pe.th32ProcessID == processId) {
                        // only get file name, like notepad.exe
                        // not full file path
                        exeFileName = pe.szExeFile;
                        break;
                    }
                } while (Process32Next(hSnapshot, &pe) != FALSE);
            }
            CloseHandle(hSnapshot);
        }
        return exeFileName;
    }


private: // data
    HWND m_hWnd;
    std::chrono::steady_clock::time_point m_lastAppendTime;

    std::wstring m_windowTitle;
    std::wstring m_exeFileName;
    std::wstring m_record;
};


class Logger {
public:
    //
    void SetTimeOut(const std::chrono::steady_clock::duration& timeOut) {
        m_timeOut = timeOut;
    }


    //
    void Append(HWND hWnd, USHORT vKey, bool isKeyDown) {
        auto timeNow = std::chrono::steady_clock::now();

        if (!m_currentRecord ||
            (hWnd != m_currentRecord->GetHWND()) ||
            (timeNow - m_currentRecord->GetLastAppendTime() > m_timeOut))
        {
            // push back old record
            if (m_currentRecord)
                m_log.push_back(*m_currentRecord);

            // new record
            m_currentRecord = std::make_shared<LogRecord>(hWnd);
        }

        if ((vKey >= '0' && vKey <= '9') || (vKey >= 'A' && vKey <= 'Z')) {
            m_currentRecord->Append(vKey);
        }
        else {
            // map virtual key code to scan code
            auto scanCode = MapVirtualKey(vKey, MAPVK_VK_TO_VSC);
            if (scanCode == 0)
                return;

            // get the representing text of a scan code
            WCHAR keyName[50]{};
            GetKeyNameText(
                scanCode << 16,
                keyName,
                sizeof(keyName) / sizeof(*keyName));
            if (isKeyDown) {
                m_currentRecord->Append(L"[");
                m_currentRecord->Append(keyName);
                m_currentRecord->Append(L"]");
            }
            else {
                m_currentRecord->Append(L"[");
                m_currentRecord->Append(L"/");
                m_currentRecord->Append(keyName);
                m_currentRecord->Append(L"]");
            }
        }
    }


    //
    std::wstring GetLog() {
        // push last record
        if (m_currentRecord) {
            m_log.push_back(*m_currentRecord);
            m_currentRecord.reset();
        }

        std::wstring log;
        for (auto& record : m_log) {
            log += record.GetRecordAsText();
            log += L"\n\n";
        }

        return log;
    }


    // clear all the log
    void Clear() {
        m_currentRecord.reset();
        m_log.clear();
    }


    //
    std::size_t GetNumberOfLogRecords() const {
        return m_log.size();
    }


private: // data
    std::chrono::steady_clock::duration m_timeOut = std::chrono::seconds(10);

    std::vector<LogRecord> m_log;
    std::shared_ptr<LogRecord> m_currentRecord;
};


Logger g_log;


int APIENTRY wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPTSTR lpCmdLine,
    _In_ int nCmdShow)
{
    try {
        Window window;

        // add message handler for WM_CREATE
        window.AddMessageHandler(
            WM_CREATE,
            [](Window *const window, UINT, WPARAM, LPARAM) -> LRESULT {
                /*
                Generic Desktop Page (0x01)
                    Usage ID
                        - Mouse (0x02)
                        - Keyboard (0x06)

                For more information
                    http://www.usb.org/developers/hidpage/Hut1_12v2.pdf
                */

                RAWINPUTDEVICE rid{};

                // add HID keyboard and also ignore legacy keyboard messages
                rid.usUsagePage = 0x01; // Generic Desktop Page
                rid.usUsage = 0x06;     // Keyboard
                rid.dwFlags = RIDEV_NOLEGACY | RIDEV_INPUTSINK; // receive all inputs
                rid.hwndTarget = window->GetHWND();

                RegisterRawInputDevices(&rid, 1, sizeof(rid));

                return 0;
            });

        // ... for WM_DESTROY
        window.AddMessageHandler(
            WM_DESTROY,
            [](Window *const window, UINT, WPARAM, LPARAM) -> LRESULT {
                OutputDebugString(g_log.GetLog().c_str());
                g_log.Clear();
                PostQuitMessage(0);
                return 0;
            });

        // ... for WM_INPUT
        window.AddMessageHandler(
            WM_INPUT,
            [](Window *const window, UINT message, WPARAM wParam, LPARAM lParam) -> LRESULT {
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
                        auto vKey = rawInput.data.keyboard.VKey;
                        auto flags = rawInput.data.keyboard.Flags;

                        bool isKeyDown =
                            flags == RI_KEY_MAKE ||
                            flags == (RI_KEY_MAKE | RI_KEY_E0) ||
                            flags == (RI_KEY_MAKE | RI_KEY_E1);
                        g_log.Append(GetForegroundWindow(), vKey, isKeyDown);
                    }
                }
                catch (...) {
                }
                return DefWindowProc(window->GetHWND(), message, wParam, lParam);
            });

        // now create window
        window.Create(L"WindowClass", L"Title");
        window.Show(nCmdShow);

        // and enter message loop
        return static_cast<int>(window.DoMessageLoop());
    }
    catch (const std::exception& e) {
        MessageBoxA(nullptr, e.what(), "Error", MB_ICONERROR);
        return -1;
    }
    catch (...) {
        MessageBox(nullptr, L"Unknown exception.", L"Error", MB_ICONERROR);
        return -1;
    }
}
