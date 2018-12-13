#ifndef GLOBAL_H
#define GLOBAL_H


#include <cstddef>


class Global {
    template<std::size_t N>
    using array = wchar_t const[N];


public:
    static array<sizeof("so61pi.Examples.ToastNotification")>&
    AppId()
    {
        static wchar_t const s_AppId[] = L"so61pi.Examples.ToastNotification";
        return s_AppId;
    }


    static array<sizeof("ToastNotification.lnk")>&
    ShortcutName()
    {
        static wchar_t const s_AppId[] = L"ToastNotification.lnk";
        return s_AppId;
    }
};


#endif // GLOBAL_H
