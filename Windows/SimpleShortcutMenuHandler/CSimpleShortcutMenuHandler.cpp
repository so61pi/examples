/*
Read more:
    Customizing a Shortcut Menu Using Dynamic Verbs
    https://msdn.microsoft.com/en-us/library/windows/desktop/ee453696(v=vs.85).aspx

    Creating Shortcut Menu Handlers
    https://msdn.microsoft.com/en-us/library/windows/desktop/cc144171(v=vs.85).aspx

    Initializing Shell Extension Handlers
    https://msdn.microsoft.com/en-us/library/windows/desktop/cc144105(v=vs.85).aspx
*/


#include "CSimpleShortcutMenuHandler.h"

#include <exception>

#include <strsafe.h>



////////////////////////////////////////////////////////////////////////////////
//
// IUnknown functions
//
////////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CSimpleShortcutMenuHandler::QueryInterface(
    REFIID riid,
    void **ppvObject)
{
    HRESULT r = S_OK;
    *ppvObject = nullptr;

    if (riid == __uuidof(IUnknown))
        *ppvObject = static_cast<IUnknown *>(static_cast<IShellExtInit *>(this));
    else if (riid == __uuidof(IShellExtInit))
        *ppvObject = static_cast<IShellExtInit *>(this);
    else if (riid == __uuidof(IContextMenu))
        *ppvObject = static_cast<IContextMenu *>(this);
    else
        r = E_NOINTERFACE;

    if (r == S_OK)
        AddRef();

    return r;
}


ULONG STDMETHODCALLTYPE CSimpleShortcutMenuHandler::AddRef() {
    ++m_RefCounter;
    return m_RefCounter;
}


ULONG STDMETHODCALLTYPE CSimpleShortcutMenuHandler::Release() {
    auto retval = --m_RefCounter;
    if (m_RefCounter == 0)
        delete this;
    return retval;
}



////////////////////////////////////////////////////////////////////////////////
//
// IShellExtInit functions
//
////////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CSimpleShortcutMenuHandler::Initialize(
    LPCITEMIDLIST pIDFolder,
    IDataObject *pDataObj,
    HKEY hRegKey)
{
    // extract the file name
    if (pDataObj) {
        STGMEDIUM medium{};
        FORMATETC fe = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };

        if (SUCCEEDED(pDataObj->GetData(&fe, &medium))) {
            // Get the count of files dropped.
            auto uCount = DragQueryFile((HDROP)medium.hGlobal, -1, NULL, 0);

            // Get the first file name from the CF_HDROP.
            if (uCount) {
                DragQueryFile((HDROP)medium.hGlobal, 0, m_FileName,
                    sizeof(m_FileName) / sizeof(TCHAR));
            }

            ReleaseStgMedium(&medium);
        }
    }

    return S_OK;
}



////////////////////////////////////////////////////////////////////////////////
//
// IContextMenu functions
//
////////////////////////////////////////////////////////////////////////////////

static const struct MenuItem {
    LPCSTR  VerbA;
    LPCSTR  HelpA;

    LPCWSTR VerbW;
    LPCWSTR HelpW;

    LPCWSTR Name;
    UINT_PTR Id;
} g_menuItems[] = {
    // first item
    { "first",  "This is the first item.",
     L"first", L"This is the first item.",
     L"First Item",
     0
    },

    // second item
    { "second",  "This is the second item.",
     L"second", L"This is the second item.",
     L"Second Item",
     1
    }
};


HRESULT STDMETHODCALLTYPE CSimpleShortcutMenuHandler::QueryContextMenu(
    HMENU hmenu,
    UINT indexMenu,
    UINT idCmdFirst,
    UINT idCmdLast,
    UINT uFlags)
{
    // if the flags include CMF_DEFAULTONLY then we shouldn't do anything
    if (uFlags & CMF_DEFAULTONLY)
        return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, 0);

    // check if we have enough space for our items
    // we have 2 items
    if (idCmdLast - idCmdFirst + 1 < 2)  {
        // then we are out of space
        return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, 0);
    }

    // insert 2 menu items
    for (auto& item : g_menuItems) {
        InsertMenu(
            hmenu,
            indexMenu++,
            MF_BYPOSITION,
            idCmdFirst + item.Id,
            item.Name);
    }

    return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, 2);
}


HRESULT STDMETHODCALLTYPE CSimpleShortcutMenuHandler::InvokeCommand(
    CMINVOKECOMMANDINFO *pici)
{
    // Read more:
    //     CMINVOKECOMMANDINFO structure
    //     https://msdn.microsoft.com/en-us/library/windows/desktop/bb773215(v=vs.85).aspx

    // the Shell only use menu-identifier offset 
    if (!IS_INTRESOURCE(pici->lpVerb))
        return E_FAIL;
    
    // get the command index
    switch (LOWORD(pici->lpVerb)) {
    case 0: case 1: {
        try {
            std::wstring msg = std::wstring(L"The selected file is:\n\n") + m_FileName;
            MessageBox(pici->hwnd, msg.c_str(), L"Simple Shorcut Menu Handler", MB_ICONINFORMATION);
        }
        catch (const std::exception& e) {
            OutputDebugStringA(e.what());
        }

        return S_OK;
    }

    default:
        return E_INVALIDARG;
    }
}


HRESULT STDMETHODCALLTYPE CSimpleShortcutMenuHandler::GetCommandString(
    UINT_PTR idCmd,
    UINT uType,
    UINT *pReserved,
    CHAR *pszName,
    UINT cchMax)
{
    // Note:
    //     http://sourceforge.net/p/pywin32/bugs/539/
    //
    //     "If GetCommandString returns the same string for all items then
    //     the shell seems to ignore all but one."  
    //
    //
    //     ValidateCommand
    //     http://blogs.msdn.com/b/oldnewthing/archive/2004/10/06/238630.aspx

    // check if idCmd is valid or not
    if (idCmd != g_menuItems[0].Id && idCmd != g_menuItems[1].Id)
        return E_INVALIDARG;

    switch (uType) {
    case GCS_HELPTEXTA:
        StringCchCopyA(pszName, cchMax, g_menuItems[idCmd].HelpA);
        break;
    case GCS_HELPTEXTW:
        StringCchCopyW(reinterpret_cast<LPWSTR>(pszName), cchMax, g_menuItems[idCmd].HelpW);
        break;

    case GCS_VERBA:
        StringCchCopyA(pszName, cchMax, g_menuItems[idCmd].VerbA);
        break;
    case GCS_VERBW:
        StringCchCopyW(reinterpret_cast<LPWSTR>(pszName), cchMax, g_menuItems[idCmd].VerbW);
        break;

    case GCS_VALIDATEA:
    case GCS_VALIDATEW:
        return S_OK;

    default:
        return E_INVALIDARG;
    }

    return S_OK;
}
