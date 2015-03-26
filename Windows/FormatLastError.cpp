#include <iostream>
#include <string>
#include <Windows.h>


std::wstring GetErrorMessage(DWORD error) {
    wchar_t *pszErrorMessage = nullptr;

    if (FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        error,
        LANG_SYSTEM_DEFAULT,
        reinterpret_cast<LPWSTR>(&pszErrorMessage),
        0,
        nullptr))
    {
        std::wstring errorMessage(pszErrorMessage);
        LocalFree(pszErrorMessage);

        return errorMessage;
    }

    return L"";
}


std::wstring GetLastErrorMessage() {
    return GetErrorMessage(GetLastError());
}


int main() {
    SetLastError(ERROR_INVALID_FUNCTION);
    std::wcout << GetErrorMessage(GetLastError());

    SetLastError(ERROR_SUCCESS);
    std::wcout << GetLastErrorMessage();
}
