#include "CMathFactory.h"


//
//
//
HRESULT STDMETHODCALLTYPE CMathFactory::QueryInterface(REFIID riid, void **ppvObject) {
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


ULONG STDMETHODCALLTYPE CMathFactory::AddRef(void) {
    // non-heap object, never delete
    return 1;
}


ULONG STDMETHODCALLTYPE CMathFactory::Release(void) {
    // non-heap object, never delete
    return 1;
}


//
//
//
HRESULT STDMETHODCALLTYPE CMathFactory::CreateInstance(_In_opt_ IUnknown *pUnkOuter, _In_ REFIID riid, _COM_Outptr_ void **ppvObject) {
    if (pUnkOuter != nullptr)
        return CLASS_E_NOAGGREGATION;

    auto math = new CMath{};
    return math->QueryInterface(riid, ppvObject);
}


HRESULT STDMETHODCALLTYPE CMathFactory::LockServer(BOOL fLock) {
    if (fLock == FALSE)
        --g_ServerCounter;
    else
        ++g_ServerCounter;
    return S_OK;
}
