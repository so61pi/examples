#include "ToastEventHandlerBase.h"


//
// DesktopToastActivatedEventHandler 
//
HRESULT STDMETHODCALLTYPE ToastEventHandlerBase::Invoke(
    _In_ ABI::Windows::UI::Notifications::IToastNotification *sender,
    _In_ ::IInspectable *args
)
{
    try
    {
        ToastActivated(sender, args);
        return S_OK;
    }
    catch (...)
    {
        return E_FAIL;
    }
}


//
// DesktopToastDismissedEventHandler
//
HRESULT STDMETHODCALLTYPE ToastEventHandlerBase::Invoke(
    _In_ ABI::Windows::UI::Notifications::IToastNotification *sender,
    _In_ ABI::Windows::UI::Notifications::IToastDismissedEventArgs *e
)
{
    try
    {
        ToastDismissed(sender, e);
        return S_OK;
    }
    catch (...)
    {
        return E_FAIL;
    }
}


//
// DesktopToastFailedEventHandler
//
HRESULT STDMETHODCALLTYPE ToastEventHandlerBase::Invoke(
    _In_ ABI::Windows::UI::Notifications::IToastNotification *sender,
    _In_ ABI::Windows::UI::Notifications::IToastFailedEventArgs *e
)
{
    try
    {
        ToastFailed(sender, e);
        return S_OK;
    }
    catch (...)
    {
        return E_FAIL;
    }
}


//
// IUnknown
//
HRESULT STDMETHODCALLTYPE ToastEventHandlerBase::QueryInterface(
    _In_ REFIID riid,
    _COM_Outptr_ void **ppv
)
{
    if (riid == __uuidof(IUnknown))
        *ppv = static_cast<IUnknown *>(static_cast<DesktopToastActivatedEventHandler *>(this));
    else if (riid == __uuidof(DesktopToastActivatedEventHandler))
        *ppv = static_cast<DesktopToastActivatedEventHandler *>(this);
    else if (riid == __uuidof(DesktopToastDismissedEventHandler))
        *ppv = static_cast<DesktopToastDismissedEventHandler *>(this);
    else if (riid == __uuidof(DesktopToastFailedEventHandler))
        *ppv = static_cast<DesktopToastFailedEventHandler *>(this);
    else
        *ppv = nullptr;

    if (*ppv) {
        AddRef();
        return S_OK;
    }

    return E_NOINTERFACE;
}


ULONG STDMETHODCALLTYPE ToastEventHandlerBase::AddRef() {
    ++m_RefCount;
    return m_RefCount;
}


ULONG STDMETHODCALLTYPE ToastEventHandlerBase::Release() {
    auto retval = --m_RefCount;
    if (m_RefCount == 0)
        delete this;
    return retval;
}
