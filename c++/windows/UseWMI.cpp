#include <iostream>
#include <Windows.h>
#include <atlbase.h>
#include <comdef.h>
#include <ShObjidl.h>
#include <Wbemidl.h>

#pragma comment(lib, "wbemuuid.lib")


/*******************************************************************************
References:

    https://msdn.microsoft.com/en-us/library/bg126473(v=vs.85).aspx
    https://msdn.microsoft.com/en-us/library/aa390423(v=vs.85).aspx

*******************************************************************************/
int main() {
    class tagCoInit {
        HRESULT m_result;

    public:
        tagCoInit() {
            m_result = CoInitialize(nullptr);
        }

        ~tagCoInit() {
            if (IsSucceeded())
                CoUninitialize();
        }

        bool IsSucceeded() const {
            return SUCCEEDED(m_result);
        }
    } CoInit;

    if (CoInit.IsSucceeded()) {
        CComPtr<IWbemLocator> pWL = nullptr;
        auto result = pWL.CoCreateInstance(__uuidof(WbemLocator));
        if (SUCCEEDED(result)) {
            CComPtr<IWbemServices> pWS = nullptr;
            result = pWL->ConnectServer(
                bstr_t(L"//./root/cimv2"),
                nullptr,
                nullptr,
                nullptr,
                0,
                nullptr,
                nullptr,
                &pWS);
            if (SUCCEEDED(result)) {
                result = CoSetProxyBlanket(
                    pWS,
                    RPC_C_AUTHN_WINNT,
                    RPC_C_AUTHZ_NONE,
                    nullptr,
                    RPC_C_AUTHN_LEVEL_CALL,
                    RPC_C_IMP_LEVEL_IMPERSONATE,
                    nullptr,
                    EOAC_NONE);
                if (SUCCEEDED(result)) {
                    CComPtr<IEnumWbemClassObject> pEWCO = nullptr;
                    result = pWS->ExecQuery(
                        bstr_t("WQL"),
                        bstr_t("SELECT * FROM Win32_CacheMemory"),
                        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
                        nullptr,
                        &pEWCO);
                    if (SUCCEEDED(result)) {
                        while (pEWCO) {
                            CComPtr<IWbemClassObject> pWCO = nullptr;
                            ULONG returnedCount = 0;

                            pEWCO->Next(WBEM_INFINITE, 1, &pWCO, &returnedCount);
                            if (returnedCount == 0)
                                break;

                            // get the value of the Purpose property
                            CComVariant val;
                            result = pWCO->Get(L"Purpose", 0, &val, 0, 0);
                            switch (val.vt) {
                            case VT_I4:
                                std::wcout << val.intVal << "\n";
                                break;

                            case VT_UI4:
                                std::wcout << val.uintVal << "\n";
                                break;

                            case VT_UI8:
                                std::wcout << val.llVal << "\n";
                                break;

                            case VT_BSTR:
                                std::wcout << val.bstrVal << "\n";
                                break;

                            case VT_NULL:
                            case VT_EMPTY:
                            default:
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
}
