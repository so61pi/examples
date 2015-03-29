#include <iostream>
#include <Windows.h>
#include <atlbase.h>
#include <ObjIdl.h>
#include <ShObjIdl.h>
#include <Shldisp.h>


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
        CComPtr<IShellDispatch6> pSD = nullptr;
        auto result = pSD.CoCreateInstance(__uuidof(Shell));
        if (SUCCEEDED(result)) {
            // open window switcher like when you press Atl + Tab
            pSD->WindowSwitcher();
        }
    }
}
