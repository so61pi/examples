#include "ToastDemo.h"

#include <string>

#include <windows.h>
#include <sal.h>
#include <windows.ui.notifications.h>

#include "Global.h"
#include "ToastEventHandler.h"


ToastDemo::ToastDemo(_In_ HWND hWnd) :
    m_toast(Global::AppId(), new ToastEventHandler{ hWnd }),
    m_shortcut(Global::AppId())
{}


void ToastDemo::CreateShortcut() {
    m_shortcut.Create(Global::ShortcutName());
}


void ToastDemo::RemoveShortcut() {
    m_shortcut.Remove();
}


void ToastDemo::Display() {
    using ABI::Windows::UI::Notifications::ToastTemplateType;

    wchar_t fullImagePath[MAX_PATH]{};
    GetFullPathName(L"green.png", ARRAYSIZE(fullImagePath), fullImagePath, nullptr);

    m_toast.SetType(ToastTemplateType::ToastTemplateType_ToastImageAndText04);
    m_toast.SetText(L"1", L"2", L"3");
    m_toast.SetImage(std::wstring{ L"file:///" } + fullImagePath);
    m_toast.SetDuration(ToastNotifier::Duration::Long);
    m_toast.SetAudio(L"ms-winsoundevent:Notification.IM");
    m_toast.DisplayToast();
}
