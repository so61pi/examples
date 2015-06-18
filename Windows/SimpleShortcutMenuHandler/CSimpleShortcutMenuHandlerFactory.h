#ifndef CSIMPLE_SHORTCUT_MENU_HANDLER_FACTORY_H
#define CSIMPLE_SHORTCUT_MENU_HANDLER_FACTORY_H


#include <atomic>
#include <Windows.h>
#include <comdef.h>
#include "CSimpleShortcutMenuHandler.h"


//
//
//
class CSimpleShortcutMenuHandlerFactory final : public IClassFactory
{
public:
    // IUnknown
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(
        REFIID riid,
        void **ppvObject) override;

    virtual ULONG STDMETHODCALLTYPE AddRef() override;

    virtual ULONG STDMETHODCALLTYPE Release() override;


    // IClassFactory
    virtual HRESULT STDMETHODCALLTYPE CreateInstance(
        IUnknown *pUnkOuter,
        REFIID riid,
        void **ppvObject) override;

    virtual HRESULT STDMETHODCALLTYPE LockServer(BOOL fLock) override;
};


#endif // CSIMPLE_SHORTCUT_MENU_HANDLER_FACTORY_H
