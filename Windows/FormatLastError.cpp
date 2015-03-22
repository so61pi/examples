#include <iostream>
#include <Windows.h>


int main() {
    SetLastError(ERROR_INVALID_FUNCTION);

    auto error = GetLastError();
    if (error != ERROR_SUCCESS) {
        wchar_t szErrorMessage[100]{};
        FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, error, LANG_SYSTEM_DEFAULT, szErrorMessage, sizeof(szErrorMessage), nullptr);
        std::wcout << szErrorMessage << "\n";
    }
}
