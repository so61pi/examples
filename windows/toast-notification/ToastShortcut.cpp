#include "ToastShortcut.h"

#include <boost/scope_exit.hpp>

#include <windows.h>
#include <Propvarutil.h>
#include <Psapi.h>
#include <sal.h>
#include <Shobjidl.h>
#include <propkey.h>
#include <wrl/client.h>

#include "HRESULTCheck.h"

#pragma comment(lib, "Shlwapi.lib")


ToastShortcut::ToastShortcut(
    _In_ string_type appId
) :
    m_appId(std::move(appId))
{
    // get exe full path
    wchar_t exePath[MAX_PATH]{};
    auto charWritten = GetModuleFileNameEx(GetCurrentProcess(), nullptr, exePath, ARRAYSIZE(exePath));
    HRESULTCheck hr = charWritten > 0 ? S_OK : E_FAIL;

    m_exePath = exePath;

    // create string to Start Menu\Programs folder
    wchar_t shortcutPath[MAX_PATH]{};
    charWritten = GetEnvironmentVariable(L"APPDATA", shortcutPath, ARRAYSIZE(shortcutPath));
    hr = charWritten > 0 ? S_OK : E_FAIL;

    m_folderPath = shortcutPath;
    m_folderPath += L"\\Microsoft\\Windows\\Start Menu\\Programs\\";
}


void ToastShortcut::Create(
    _In_ string_type fileName
)
{
    // remove old shortcut
    if (m_fileName.size())
        Remove();

    m_fileName = std::move(fileName);

    auto shortcutPath = m_folderPath + m_fileName;
    bool fileExists = PathFileExists(shortcutPath.c_str()) != FALSE;
    if (!fileExists) {
        Install(shortcutPath);
    }
}


void ToastShortcut::Remove() {
    if (m_fileName.size())
        DeleteFile((m_folderPath + m_fileName).c_str());
    m_fileName.clear();
}


void ToastShortcut::Install(
    _In_ string_type const& shortcutPath
) const
{
    using Microsoft::WRL::ComPtr;

    ComPtr<IShellLink> shellLink;
    HRESULTCheck hr = CoCreateInstance(__uuidof(ShellLink), nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&shellLink));

    hr = shellLink->SetPath(m_exePath.c_str());

    // set property for shortcut
    ComPtr<IPropertyStore> propertyStore;
    hr = shellLink.As(&propertyStore);

    PROPVARIANT appIdPropVar;
    hr = InitPropVariantFromString(m_appId.c_str(), &appIdPropVar);

    BOOST_SCOPE_EXIT(&appIdPropVar) {
        PropVariantClear(&appIdPropVar);
    } BOOST_SCOPE_EXIT_END;

    hr = propertyStore->SetValue(PKEY_AppUserModel_ID, appIdPropVar);
    hr = propertyStore->Commit();

    // save shortcut to file
    ComPtr<IPersistFile> persistFile;
    hr = shellLink.As(&persistFile);
    hr = persistFile->Save(shortcutPath.c_str(), TRUE);
}
