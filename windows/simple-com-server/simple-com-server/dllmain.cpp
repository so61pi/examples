#include <exception>
#include <stdexcept>
#include <string>

#include <Windows.h>
#include <strsafe.h>
#include <comdef.h>

#include "IMath.h"
#include "CMathFactory.h"
#include "Global.h"


//
//
//
std::wstring GetRegSubKeyFromCLSID(REFCLSID rclsid) {
    // convert CLSID to string
    //
    // NOTE: StringFromCLSID's returned string already has open and close bracket
    // e.g. {F6124E58-C6FB-4BF7-BA21-21849D93C43F}
    LPOLESTR CLSIDString = nullptr;
    if (FAILED(StringFromCLSID(rclsid, &CLSIDString)))
        throw std::runtime_error("Cannot create sub key from CLSID.");

    // create Registry sub key
    wchar_t subKey[100]{};
    if (FAILED(StringCbPrintf(subKey, sizeof(subKey), L"CLSID\\%s", CLSIDString)))
        throw std::runtime_error("Cannot create sub key from CLSID.");

    // free memory return from StringFromCLSID
    CoTaskMemFree(CLSIDString);

    return subKey;
}


//
//
//
std::wstring GetThisDllFilePath() {
    HMODULE hMod = nullptr;
    if (GetModuleHandleEx(
        GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
        reinterpret_cast<LPCWSTR>(&GetThisDllFilePath),
        &hMod) == FALSE)
    {
        throw std::runtime_error("Cannot get DLL file path.");
    }

    // get dll full file path
    wchar_t dllFilePath[MAX_PATH]{};
    auto result = GetModuleFileName(hMod, dllFilePath, MAX_PATH);
    if (result == 0 || GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
        throw std::runtime_error("Cannot get DLL file path.");
    }

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

    if (rclsid == __uuidof(CMath)) {
        // get CMath factory
        static CMathFactory s_CMathFactory;

        if (riid == __uuidof(IClassFactory)) {
            *ppv = &s_CMathFactory;
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
    try {
        auto dllFilePath = GetThisDllFilePath();
        auto subKey = GetRegSubKeyFromCLSID(__uuidof(CMath)) + L"\\InprocServer32";
        RegSetKeyValue(
            HKEY_CLASSES_ROOT,
            subKey.c_str(),
            nullptr,
            REG_SZ,
            dllFilePath.c_str(),
            dllFilePath.size() * sizeof(wchar_t));
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
        RegDeleteTree(
            HKEY_CLASSES_ROOT,
            GetRegSubKeyFromCLSID(__uuidof(CMath)).c_str());
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
