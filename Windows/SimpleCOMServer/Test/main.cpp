#include <iostream>
#include <Windows.h>
#include <atlbase.h>
#include "../SimpleCOMServer/IMath.h"


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
        CComPtr<IAdd> pAdd = nullptr;
        CComPtr<ISub> pSub = nullptr;
        auto result = pAdd.CoCreateInstance(__uuidof(CMath));
        if (SUCCEEDED(result)) {
            LONG a = 1;
            LONG b = 2;
            LONG c{};

            pAdd->Add(a, b, &c);
            std::cout << c << "\n";
            
            pAdd->QueryInterface(__uuidof(ISub), reinterpret_cast<void **>(&pSub));

            pSub->Sub(a, b, &c);
            std::cout << c << "\n";
        }
    }
}
