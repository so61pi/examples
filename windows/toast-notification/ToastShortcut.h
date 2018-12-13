#ifndef TOAST_SHORTCUT_H
#define TOAST_SHORTCUT_H


#include <string>

#include <sal.h>


//
// In order to display toasts, a desktop application must have a shortcut on the Start menu.
// Also, an AppUserModelID must be set on that shortcut.
//
// read more at:
//     https://msdn.microsoft.com/en-us/library/windows/desktop/hh802762%28v=vs.85%29.aspx
//
class ToastShortcut final {
    using string_type = std::wstring;


public:
    ToastShortcut(
        _In_ string_type appId
    );

    void Create(
        _In_ string_type fileName
    );

    void Remove();


private:
    void Install(
        _In_ string_type const& shortcutPath
    ) const;


private:
    string_type const m_appId;
    string_type m_exePath;
    string_type m_folderPath;
    string_type m_fileName;
};


#endif // TOAST_SHORTCUT_H
