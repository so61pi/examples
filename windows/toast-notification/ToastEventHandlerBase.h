#ifndef TOAST_EVENT_HANDLER_BASE_H
#define TOAST_EVENT_HANDLER_BASE_H


#include <atomic>
#include <functional>

#include <windows.h>
#include <inspectable.h>
#include <sal.h>
#include <windows.foundation.h>
#include <windows.ui.notifications.h>
#include <wrl/implements.h>


using DesktopToastActivatedEventHandler = ABI::Windows::Foundation::ITypedEventHandler<ABI::Windows::UI::Notifications::ToastNotification *, ::IInspectable *>;
using DesktopToastDismissedEventHandler = ABI::Windows::Foundation::ITypedEventHandler<ABI::Windows::UI::Notifications::ToastNotification *, ABI::Windows::UI::Notifications::ToastDismissedEventArgs *>;
using DesktopToastFailedEventHandler = ABI::Windows::Foundation::ITypedEventHandler<ABI::Windows::UI::Notifications::ToastNotification *, ABI::Windows::UI::Notifications::ToastFailedEventArgs *>;
using DesktopToastEventHandler = Microsoft::WRL::Implements<DesktopToastActivatedEventHandler, DesktopToastDismissedEventHandler, DesktopToastFailedEventHandler>;


// basically this class is a COM class which has a delete statement in Release() function
// therefore all the derived classes must be created on heap
class ToastEventHandlerBase : public DesktopToastEventHandler
{
private:

    // derived classes only need to implements 3 functions below

    virtual void ToastActivated(
        _In_ ABI::Windows::UI::Notifications::IToastNotification *sender,
        _In_::IInspectable *args
    ) = 0;

    virtual void ToastDismissed(
        _In_ ABI::Windows::UI::Notifications::IToastNotification *sender,
        _In_ ABI::Windows::UI::Notifications::IToastDismissedEventArgs *e
    ) = 0;

    virtual void ToastFailed(
        _In_ ABI::Windows::UI::Notifications::IToastNotification *sender,
        _In_ ABI::Windows::UI::Notifications::IToastFailedEventArgs *e
    ) = 0;


public:
    //
    // DesktopToastActivatedEventHandler 
    //
    virtual HRESULT STDMETHODCALLTYPE Invoke(
        _In_ ABI::Windows::UI::Notifications::IToastNotification *sender,
        _In_ ::IInspectable *args
    ) override final;


    //
    // DesktopToastDismissedEventHandler
    //
    virtual HRESULT STDMETHODCALLTYPE Invoke(
        _In_ ABI::Windows::UI::Notifications::IToastNotification *sender,
        _In_ ABI::Windows::UI::Notifications::IToastDismissedEventArgs *e
    ) override final;


    //
    // DesktopToastFailedEventHandler
    //
    virtual HRESULT STDMETHODCALLTYPE Invoke(
        _In_ ABI::Windows::UI::Notifications::IToastNotification *sender,
        _In_ ABI::Windows::UI::Notifications::IToastFailedEventArgs *e
    ) override final;


    //
    // IUnknown
    //
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(
        _In_ REFIID riid,
        _COM_Outptr_ void **ppv
    ) override final;

    virtual ULONG STDMETHODCALLTYPE AddRef() override final;

    virtual ULONG STDMETHODCALLTYPE Release() override final;


private:
    std::atomic<ULONG> m_RefCount = 0;
};


#endif // TOAST_EVENT_HANDLER_BASE_H
