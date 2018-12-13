#ifndef STRING_WRAPPER_H
#define STRING_WRAPPER_H


#include <cstddef>
#include <string>

#include <Hstring.h>
#include <sal.h>
#include <windows.h>
#include <winstring.h>

#include "HRESULTCheck.h"

#pragma comment(lib, "RuntimeObject.lib")
#pragma comment(lib, "Shlwapi.lib")


class StringWrapper final {
public:
    StringWrapper(
        _In_ std::wstring const& str
    )
    {
        HRESULTCheck hr = WindowsCreateString(str.c_str(), str.size(), &m_hString);
    }

    StringWrapper(
        _In_reads_(length) wchar_t const *str,
        _In_ std::size_t length
    )
    {
        HRESULTCheck hr = WindowsCreateString(str, length, &m_hString);
    }

    ~StringWrapper() {
        WindowsDeleteString(m_hString);
    }

    HSTRING Get() const {
        return m_hString;
    }

private:
    HSTRING m_hString;
};


class StringReferenceWrapper final {
public:
    template<std::size_t N>
    StringReferenceWrapper(
        _In_reads_(N) wchar_t const (&str)[N]
    )
    {
        HRESULTCheck hr = WindowsCreateStringReference(str, N - 1, &m_hStringHeader, &m_hString);
    }

    ~StringReferenceWrapper() {
        WindowsDeleteString(m_hString);
    }

    HSTRING Get() const {
        return m_hString;
    }

private:
    HSTRING m_hString;
    HSTRING_HEADER m_hStringHeader;
};


#endif // STRING_WRAPPER_H
