#include "CSimpleShortcutMenuHandlerFactory.h"

#include "Global.h"


//
//
//
HRESULT STDMETHODCALLTYPE CSimpleShortcutMenuHandlerFactory::QueryInterface(
    REFIID riid,
    void **ppvObject)
{
    HRESULT r = S_OK;
    *ppvObject = nullptr;

    if (riid == __uuidof(IUnknown))
        *ppvObject = static_cast<IUnknown *>(this);
    else if (riid == __uuidof(IClassFactory))
        *ppvObject = static_cast<IClassFactory *>(this);
    else
        r = E_NOINTERFACE;

    if (r == S_OK)
        AddRef();

    return r;
}


ULONG STDMETHODCALLTYPE CSimpleShortcutMenuHandlerFactory::AddRef() {
    return 1;
}


ULONG STDMETHODCALLTYPE CSimpleShortcutMenuHandlerFactory::Release() {
    return 1;
}


//
//
//
HRESULT STDMETHODCALLTYPE CSimpleShortcutMenuHandlerFactory::CreateInstance(
    IUnknown *pUnkOuter, 
    REFIID riid, 
    void **ppvObject)
{
    if (pUnkOuter != nullptr)
        return CLASS_E_NOAGGREGATION;

    auto ext = new CSimpleShortcutMenuHandler{};
    return ext->QueryInterface(riid, ppvObject);
}


HRESULT STDMETHODCALLTYPE CSimpleShortcutMenuHandlerFactory::LockServer(BOOL fLock) {
    if (fLock)
        ++g_ServerCounter;
    else
        --g_ServerCounter;

    return S_OK;
}
