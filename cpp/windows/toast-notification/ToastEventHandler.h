#ifndef TOAST_EVENT_HANDLER_H
#define TOAST_EVENT_HANDLER_H


#include <windows.h>
#include <sal.h>
#include <inspectable.h>
#include <windows.ui.notifications.h>

#include "ToastEventHandlerBase.h"


class ToastEventHandler final : public ToastEventHandlerBase
{
public:
    ToastEventHandler(_In_ HWND hWnd);


private:
    virtual void ToastActivated(
        _In_ ABI::Windows::UI::Notifications::IToastNotification *sender,
        _In_ ::IInspectable *args
    ) override;

    virtual void ToastDismissed(
        _In_ ABI::Windows::UI::Notifications::IToastNotification *sender,
        _In_ ABI::Windows::UI::Notifications::IToastDismissedEventArgs *e
    ) override;

    virtual void ToastFailed(
        _In_ ABI::Windows::UI::Notifications::IToastNotification *sender,
        _In_ ABI::Windows::UI::Notifications::IToastFailedEventArgs *e
    ) override;


private:
    HWND const m_hWnd;
};


#endif // TOAST_EVENT_HANDLER_H
