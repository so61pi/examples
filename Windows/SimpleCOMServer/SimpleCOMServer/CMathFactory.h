#ifndef CMATH_FACTORY_H
#define CMATH_FACTORY_H


#include <atomic>
#include <Windows.h>
#include <comdef.h>
#include "CMath.h"
#include "Global.h"


//
//
//
class CMathFactory : public IClassFactory {
public:
    // IUnknown
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject) override;
    virtual ULONG STDMETHODCALLTYPE AddRef(void) override;
    virtual ULONG STDMETHODCALLTYPE Release(void) override;

    // IClassFactory
    virtual HRESULT STDMETHODCALLTYPE CreateInstance(_In_opt_ IUnknown *pUnkOuter, _In_ REFIID riid, _COM_Outptr_ void **ppvObject) override;
    virtual HRESULT STDMETHODCALLTYPE LockServer(BOOL fLock) override;

private:
    std::atomic<long long> m_RefCount = 0;
};


#endif // CMATH_FACTORY_H
