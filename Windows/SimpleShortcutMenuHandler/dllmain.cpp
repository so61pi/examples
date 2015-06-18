#include <exception>
#include <stdexcept>
#include <string>

#include <Windows.h>
#include <strsafe.h>
#include <comdef.h>

#include "Global.h"
#include "CSimpleShortcutMenuHandlerFactory.h"


//
//
//
std::wstring GetCLSIDString(REFCLSID rclsid) {
    // convert CLSID to string
    //
    // NOTE: StringFromCLSID's returned string already has open and close bracket
    // e.g. {F6124E58-C6FB-4BF7-BA21-21849D93C43F}
    LPOLESTR CLSIDString = nullptr;
    if (FAILED(StringFromCLSID(rclsid, &CLSIDString)))
        throw std::runtime_error("Cannot create sub key from CLSID.");

    std::wstring s = CLSIDString;

    // free memory return from StringFromCLSID
    CoTaskMemFree(CLSIDString);

    return s;
}


//
//
//
std::wstring GetRegSubKeyFromCLSID(REFCLSID rclsid) {
    return L"CLSID\\" + GetCLSIDString(rclsid);
}


//
//
//
std::wstring GetThisDllFilePath() {
    HMODULE hMod = nullptr;
    if (GetModuleHandleEx(
            GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
            reinterpret_cast<LPCWSTR>(&GetThisDllFilePath),
            &hMod)
        == FALSE)
    {
        throw std::runtime_error("Cannot get DLL file path.");
    }

    // get dll full file path
    wchar_t dllFilePath[MAX_PATH]{};
    auto result = GetModuleFileName(hMod, dllFilePath, MAX_PATH);
    if (result == 0 || GetLastError() == ERROR_INSUFFICIENT_BUFFER)
        throw std::runtime_error("Cannot get DLL file path.");

    return dllFilePath;
}


BOOL WINAPI DllMain(
    HINSTANCE hinstDLL, // handle to DLL module
    DWORD fdwReason,    // reason for calling function
    LPVOID lpReserved)  // reserved
{
    // Perform actions based on the reason for calling.
    switch (fdwReason) {
    case DLL_PROCESS_ATTACH:
        // Initialize once for each new process.
        // Return FALSE to fail DLL load.
        break;

    case DLL_THREAD_ATTACH:
        // Do thread-specific initialization.
        break;

    case DLL_THREAD_DETACH:
        // Do thread-specific cleanup.
        break;

    case DLL_PROCESS_DETACH:
        // Perform any necessary cleanup.
        break;
    }
    return TRUE;  // Successful DLL_PROCESS_ATTACH.
}


//
//
//
HRESULT __stdcall DllGetClassObject(
    REFCLSID rclsid,
    REFIID riid,
    LPVOID *ppv)
{
    *ppv = nullptr;

    if (rclsid == __uuidof(CSimpleShortcutMenuHandler)) {
        static CSimpleShortcutMenuHandlerFactory s_CSimpleShellExtensionFactory;

        if (riid == __uuidof(IClassFactory)) {
            *ppv = reinterpret_cast<LPVOID>(&s_CSimpleShellExtensionFactory);
            return S_OK;
        }
    }

    return CLASS_E_CLASSNOTAVAILABLE;
}


//
//
//
HRESULT __stdcall DllCanUnloadNow() {
    if (g_ServerCounter <= 0)
        return S_OK;
    else
        return S_FALSE;
}


//
//
//
HRESULT __stdcall DllRegisterServer() {
    // Read more:
    //     Registering Shell Extension Handlers
    //     https://msdn.microsoft.com/en-us/library/windows/desktop/cc144110(v=vs.85).aspx

    try {
        // add necessary Registry keys
        auto dllFilePath = GetThisDllFilePath();
        auto subKey = GetRegSubKeyFromCLSID(__uuidof(CSimpleShortcutMenuHandler)) + L"\\InprocServer32";
        RegSetKeyValue(
            HKEY_CLASSES_ROOT,
            subKey.c_str(),
            nullptr,
            REG_SZ,
            dllFilePath.c_str(),
            (dllFilePath.size() + 1) * sizeof(wchar_t));

        RegSetKeyValue(
            HKEY_CLASSES_ROOT,
            subKey.c_str(),
            L"ThreadingModel",
            REG_SZ,
            L"Apartment",
            sizeof(L"Apartment"));

        auto clsidString = GetCLSIDString(__uuidof(CSimpleShortcutMenuHandler));
        RegSetKeyValue(
            HKEY_CLASSES_ROOT,
            L"txtfile\\ShellEx\\ContextMenuHandlers\\SimpleShortcutMenuHandler",
            nullptr,
            REG_SZ,
            clsidString.c_str(),
            (clsidString.size() + 1) * sizeof(wchar_t));

        // notify system
        SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, nullptr, nullptr);

        return S_OK;
    }
    catch (const std::exception& e) {
        OutputDebugStringA(e.what());
        return SELFREG_E_CLASS;
    }
    catch (...) {
        return SELFREG_E_CLASS;
    }
}


//
//
//
HRESULT __stdcall DllUnregisterServer() {
    try {
        // remove all added Registry keys
        RegDeleteTree(
            HKEY_CLASSES_ROOT,
            GetRegSubKeyFromCLSID(__uuidof(CSimpleShortcutMenuHandler)).c_str());

        RegDeleteTree(
            HKEY_CLASSES_ROOT,
            L"txtfile\\ShellEx\\ContextMenuHandlers\\SimpleShortcutMenuHandler");

        // notify system
        SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, nullptr, nullptr);

        return S_OK;
    }
    catch (const std::exception& e) {
        OutputDebugStringA(e.what());
        return SELFREG_E_CLASS;
    }
    catch (...) {
        return SELFREG_E_CLASS;
    }
}
