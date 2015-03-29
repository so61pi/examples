#include <iostream>
#include <Windows.h>
#include <WinCred.h>

#pragma comment(lib, "Credui.lib")


int main() {
    CREDUI_INFO ui{};
    ui.cbSize = sizeof(ui);
    ui.pszCaptionText = L"Authentication";
    ui.pszMessageText = L"Please give me your user name and password.";
    ULONG authenticationPackage{};
    PVOID authBuffer{};
    ULONG authBufferSize{};
    BOOL saveCheckBox = FALSE;

    auto result = CredUIPromptForWindowsCredentials(
        &ui,
        ERROR_SUCCESS,
        &authenticationPackage,
        nullptr,
        0,
        &authBuffer,
        &authBufferSize,
        &saveCheckBox,
        CREDUIWIN_GENERIC | CREDUIWIN_CHECKBOX);
    if (result == NO_ERROR)
    {
        WCHAR userName[CREDUI_MAX_USERNAME_LENGTH + 1]{};
        WCHAR password[CREDUI_MAX_PASSWORD_LENGTH + 1]{};
        WCHAR domain[CREDUI_MAX_DOMAIN_TARGET_LENGTH + 1]{};
        DWORD maxUserNameSize = sizeof(userName);
        DWORD maxPasswordSize = sizeof(password);
        DWORD maxDomainSize = sizeof(domain);

        CredUnPackAuthenticationBuffer(
            0,
            authBuffer,
            authBufferSize,
            userName,
            &maxUserNameSize,
            domain,
            &maxDomainSize,
            password,
            &maxPasswordSize);

        SecureZeroMemory(authBuffer, authBufferSize);
        CoTaskMemFree(authBuffer);

        std::wcout << "Domain: " << domain << "\n";
        std::wcout << "User Name: " << userName << "\n";
        std::wcout << "Password: " << password << "\n";

        SecureZeroMemory(domain, sizeof(domain));
        SecureZeroMemory(userName, sizeof(userName));
        SecureZeroMemory(password, sizeof(password));
    }
}
