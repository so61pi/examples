#include <iostream>
#include <Windows.h>
#include <Ntsecapi.h>
#include <WinCred.h>
#include <ntstatus.h>

#pragma comment(lib, "Secur32.lib")
#pragma comment(lib, "Credui.lib")


int main() {
    HANDLE lsaHandle{};
    NTSTATUS status = LsaConnectUntrusted(&lsaHandle);
    if (status != STATUS_SUCCESS)
        return status;

    LSA_STRING packageName{};
    packageName.Length = sizeof(MSV1_0_PACKAGE_NAME) - 1;
    packageName.MaximumLength = sizeof(MSV1_0_PACKAGE_NAME) - 1;
    packageName.Buffer = MSV1_0_PACKAGE_NAME;
    ULONG authenticationPackage{};
    status = LsaLookupAuthenticationPackage(lsaHandle, &packageName, &authenticationPackage);
    if (status != STATUS_SUCCESS) {
        LsaDeregisterLogonProcess(lsaHandle);
        return status;
    }

    CREDUI_INFO ui{};
    ui.cbSize = sizeof(ui);
    ui.pszCaptionText = L"Authentication";
    ui.pszMessageText = L"Please give me your user name and password.";

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
        CREDUIWIN_AUTHPACKAGE_ONLY | CREDUIWIN_CHECKBOX);
    if (result == NO_ERROR)
    {
        WCHAR userName[CREDUI_MAX_USERNAME_LENGTH + 1]{};
        WCHAR password[CREDUI_MAX_PASSWORD_LENGTH + 1]{};
        WCHAR domain[CREDUI_MAX_DOMAIN_TARGET_LENGTH + 1]{};
        DWORD maxUserNameSize = sizeof(userName);
        DWORD maxPasswordSize = sizeof(password);
        DWORD maxDomainSize = sizeof(domain);

        CredUnPackAuthenticationBuffer(
            CRED_PACK_PROTECTED_CREDENTIALS,
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

    return LsaDeregisterLogonProcess(lsaHandle);
}
