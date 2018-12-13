#include "ToastEventHandler.h"

#include <string>

#include <windows.h>


ToastEventHandler::ToastEventHandler(_In_ HWND hWnd) :
    m_hWnd(hWnd)
{}


void ToastEventHandler::ToastActivated(
    _In_ ABI::Windows::UI::Notifications::IToastNotification * /* sender */,
    _In_ ::IInspectable * /* args */
)
{
    SetWindowText(m_hWnd, L"Toast Activated");
    SetForegroundWindow(m_hWnd);
}


void ToastEventHandler::ToastDismissed(
    _In_ ABI::Windows::UI::Notifications::IToastNotification * /* sender */,
    _In_ ABI::Windows::UI::Notifications::IToastDismissedEventArgs *e
)
{
    using ABI::Windows::UI::Notifications::ToastDismissalReason;

    ToastDismissalReason tdr{};
    HRESULT hr = e->get_Reason(&tdr);
    if (SUCCEEDED(hr)) {
        wchar_t *outputText = nullptr;

        switch (tdr) {
        case ToastDismissalReason::ToastDismissalReason_ApplicationHidden:
            outputText = L"Toast Dismissed : The application hid the toast using ToastNotifier.hide()";
            break;

        case ToastDismissalReason::ToastDismissalReason_UserCanceled:
            outputText = L"Toast Dismissed : The user dismissed this toast";
            break;

        case ToastDismissalReason::ToastDismissalReason_TimedOut:
            outputText = L"Toast Dismissed : The toast has timed out";
            break;

        default:
            outputText = L"Toast Dismissed : Toast not activated";
            break;
        }

        SetWindowText(m_hWnd, outputText);
    }
}


void ToastEventHandler::ToastFailed(
    _In_ ABI::Windows::UI::Notifications::IToastNotification * /* sender */,
    _In_ ABI::Windows::UI::Notifications::IToastFailedEventArgs *e
)
{
    try {
        HRESULT ec{};
        e->get_ErrorCode(&ec);
        auto outputText = L"Toast Failed : " + std::to_wstring(ec);
        SetWindowText(m_hWnd, outputText.c_str());
    }
    catch (...) {
        SetWindowText(m_hWnd, L"Toast Failed");
    }
}
