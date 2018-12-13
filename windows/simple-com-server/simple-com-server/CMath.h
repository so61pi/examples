#ifndef CMATH_H
#define CMATH_H


#include <atomic>
#include <Windows.h>
#include "IMath.h"


//
//
//
class CMath : public IAdd, public ISub, public IMul, public IDiv {
public:
    // IUnknown
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject) override;
    virtual ULONG STDMETHODCALLTYPE AddRef(void) override;
    virtual ULONG STDMETHODCALLTYPE Release(void) override;

    // IAdd
    virtual HRESULT STDMETHODCALLTYPE Add(LONG a, LONG b, LONG *c) override;

    // ISub
    virtual HRESULT STDMETHODCALLTYPE Sub(LONG a, LONG b, LONG *c) override;

    // IMul
    virtual HRESULT STDMETHODCALLTYPE Mul(LONG a, LONG b, LONG *c) override;

    // IDiv
    virtual HRESULT STDMETHODCALLTYPE Div(LONG a, LONG b, LONG *c) override;

private:
    std::atomic<long long> m_RefCount = 0;
};


#endif // CMATH_H
