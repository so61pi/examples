#include <iostream>
#include <Windows.h>
#include <atlbase.h>
#include <ShObjIdl.h>
#include <ObjIdl.h>


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
        CComPtr<IShellLink> pSL = nullptr;
        auto result = pSL.CoCreateInstance(__uuidof(ShellLink));
        if (SUCCEEDED(result)) {
            // something
        }
    }
}
