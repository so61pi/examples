#include <iostream>
#include <Windows.h>
#include <atlbase.h>
#include <comdef.h>
#include <ShObjidl.h>
#include <Wbemidl.h>

#pragma comment(lib, "wbemuuid.lib")


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
        //
        // error checking is ignored
        //

        CComPtr<IWbemLocator> pWL = nullptr;
        auto result = pWL.CoCreateInstance(__uuidof(WbemLocator));
        CComPtr<IWbemServices> pWS = nullptr;
        result = pWL->ConnectServer(
            bstr_t(L"root\\cimv2\\Security\\MicrosoftVolumeEncryption"),
            nullptr, nullptr, nullptr, 0, nullptr, nullptr, &pWS);
        result = CoSetProxyBlanket(pWS, RPC_C_AUTHN_WINNT,
            RPC_C_AUTHZ_NONE, nullptr, RPC_C_AUTHN_LEVEL_CALL,
            RPC_C_IMP_LEVEL_IMPERSONATE, nullptr, EOAC_NONE);
        CComPtr<IEnumWbemClassObject> pEWCO = nullptr;
        result = pWS->ExecQuery(bstr_t("WQL"),
            bstr_t("SELECT * FROM Win32_EncryptableVolume WHERE DriveLetter='X:'"),
            WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
            nullptr, &pEWCO);

        if (pEWCO) {
            CComPtr<IWbemClassObject> pWCO = nullptr;
            ULONG returnedCount = 0;
            result = pEWCO->Next(WBEM_INFINITE, 1, &pWCO, &returnedCount);
            if (returnedCount != 0) {
                CComVariant objPath;
                result = pWCO->Get(L"__PATH", 0, &objPath, 0, 0);
                std::wcout << "Object path : " << objPath.bstrVal << "\n";

                wchar_t className[] = L"Win32_EncryptableVolume";
                wchar_t methodName[] = L"UnlockWithPassphrase";

                CComPtr<IWbemClassObject> pCO = nullptr;
                result = pWS->GetObjectW(className, 0, nullptr, &pCO, nullptr);

                CComPtr<IWbemClassObject> pInSignatureClass = nullptr;
                result = pCO->GetMethod(methodName, 0, &pInSignatureClass, nullptr);

                CComPtr<IWbemClassObject> pInParams = nullptr;
                result = pInSignatureClass->SpawnInstance(0, &pInParams);

                CComVariant password;
                password.vt = VT_BSTR;
                password.bstrVal = bstr_t(L"MyPassword");
                result = pInParams->Put(L"Passphrase", 0, &password, 0);

                CComPtr<IWbemClassObject> pOutParams = nullptr;
                result = pWS->ExecMethod(objPath.bstrVal, methodName, 0, nullptr, pInParams, &pOutParams, nullptr);

                CComVariant rescode;
                rescode.vt = VT_UNKNOWN;
                result = pOutParams->Get(L"ReturnValue", 0, &rescode, nullptr, nullptr);
                std::cout << "Result : " << rescode.intVal;
            }
        }
    }
}
