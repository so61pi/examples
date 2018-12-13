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
        // create an instance of ShellLink interface to set
        // shortcut information (path, description, working directory...)
        CComPtr<IShellLink> pSL = nullptr;
        auto result = pSL.CoCreateInstance(__uuidof(ShellLink));
        if (SUCCEEDED(result)) {
            pSL->SetPath(L"C:\\Windows");

            // get PersistFile interface to save shortcut to file
            CComPtr<IPersistFile> pPL = nullptr;
            result = pSL->QueryInterface(IID_IPersistFile, reinterpret_cast<void **>(&pPL.p));
            if (SUCCEEDED(result)) {
                result = pPL->Save(L"D:\\windows.lnk", TRUE);
                if (SUCCEEDED(result)) {
                    MessageBox(nullptr, L"Shortcut is created successfully.", L"Message", MB_ICONINFORMATION);
                }
            }
        }
    }
}
