#include "CMath.h"


//
// IUnknown
//
HRESULT STDMETHODCALLTYPE CMath::QueryInterface(REFIID riid, void **ppvObject) {
    HRESULT r = S_OK;
    *ppvObject = nullptr;

    if (riid == __uuidof(IUnknown))
        *ppvObject = static_cast<IUnknown *>(static_cast<IAdd *>(this));
    else if (riid == __uuidof(IAdd))
        *ppvObject = static_cast<IAdd *>(this);
    else if (riid == __uuidof(ISub))
        *ppvObject = static_cast<ISub *>(this);
    else if (riid == __uuidof(IMul))
        *ppvObject = static_cast<IMul *>(this);
    else if (riid == __uuidof(IDiv))
        *ppvObject = static_cast<IDiv *>(this);
    else
        r = E_NOINTERFACE;

    if (r == S_OK)
        AddRef();

    return r;
}


ULONG STDMETHODCALLTYPE CMath::AddRef(void) {
    ++m_RefCount;
    return static_cast<ULONG>(m_RefCount);
}


ULONG STDMETHODCALLTYPE CMath::Release(void) {
    auto retval = --m_RefCount;
    if (m_RefCount == 0)
        delete this;
    return static_cast<ULONG>(retval);
}


//
// IAdd
//
HRESULT STDMETHODCALLTYPE CMath::Add(LONG a, LONG b, LONG *c) {
    *c = a + b;
    return S_OK;
}


//
// ISub
//
HRESULT STDMETHODCALLTYPE CMath::Sub(LONG a, LONG b, LONG *c) {
    *c = a - b;
    return S_OK;
}


//
// IMul
//
HRESULT STDMETHODCALLTYPE CMath::Mul(LONG a, LONG b, LONG *c) {
    *c = a * b;
    return S_OK;
}


//
// IDiv
//
HRESULT STDMETHODCALLTYPE CMath::Div(LONG a, LONG b, LONG *c) {
    *c = a / b;
    return S_OK;
}
