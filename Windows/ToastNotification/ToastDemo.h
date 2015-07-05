#ifndef TOAST_DEMO_H
#define TOAST_DEMO_H


#include <windows.h>
#include <sal.h>

#include "ToastNotifier.h"
#include "ToastShortcut.h"


class ToastDemo final {
public:
    ToastDemo(_In_ HWND hWnd);

    void CreateShortcut();
    void RemoveShortcut();
    void Display();

private:
    ToastNotifier m_toast;
    ToastShortcut m_shortcut;
};


#endif // TOAST_DEMO_H
