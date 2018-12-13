#ifndef CSIMPLE_SHORTCUT_MENU_HANDLER_H
#define CSIMPLE_SHORTCUT_MENU_HANDLER_H


#include <string>

#include <Windows.h>
#include <shlobj.h>
#include <comdef.h>


class DECLSPEC_UUID("65e0609a-95b6-4b20-8a3d-9f92869aa8f0")
CSimpleShortcutMenuHandler final :
    public IShellExtInit,
    public IContextMenu
{
public:
    // IUnknown
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(
        REFIID riid,
        void **ppvObject) override;

    virtual ULONG STDMETHODCALLTYPE AddRef() override;

    virtual ULONG STDMETHODCALLTYPE Release() override;


    // IShellExtInit
    virtual HRESULT STDMETHODCALLTYPE Initialize(
        LPCITEMIDLIST pIDFolder,
        IDataObject *pDataObj,
        HKEY hRegKey) override;


    // IContextMenu
    virtual HRESULT STDMETHODCALLTYPE QueryContextMenu(
        HMENU hmenu,
        UINT indexMenu,
        UINT idCmdFirst,
        UINT idCmdLast,
        UINT uFlags) override;

    virtual HRESULT STDMETHODCALLTYPE InvokeCommand(
        CMINVOKECOMMANDINFO *pici) override;

    virtual HRESULT STDMETHODCALLTYPE GetCommandString(
        UINT_PTR idCmd,
        UINT uType,
        UINT *pReserved,
        CHAR *pszName,
        UINT cchMax) override;


private: // data
    int m_RefCounter = 0;

    WCHAR m_FileName[MAX_PATH];
};


#endif // CSIMPLE_SHORTCUT_MENU_HANDLER_H
