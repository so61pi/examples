#include <exception>
#include <iostream>
#include <stdexcept>
#include <Windows.h>
#include <atlbase.h>
#include <ObjIdl.h>
#include <ShObjIdl.h>


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

    try {
        if (!CoInit.IsSucceeded())
            throw std::runtime_error("Cannot init COM.");

        // create a FileOpenDialog instance
        CComPtr<IFileOpenDialog> pFileOpenDialog = nullptr;
        auto result = pFileOpenDialog.CoCreateInstance(__uuidof(FileOpenDialog));
        if (FAILED(result))
            throw std::runtime_error("Cannot get IFileOpenDialog interface.");

        // set dialog options
        FILEOPENDIALOGOPTIONS options{};
        pFileOpenDialog->GetOptions(&options);
        pFileOpenDialog->SetOptions(options | FOS_ALLOWMULTISELECT);

        // set file type filter
        COMDLG_FILTERSPEC fileTypeFilter[] {
            { L"All files", L"*.*" },
            { L"Text",      L"*.txt" },
            { L"Word",      L"*.doc;*.docx" },
            { L"Excel",     L"*.xls;*.xlsx" }
        };
        pFileOpenDialog->SetFileTypes(sizeof(fileTypeFilter) / sizeof(*fileTypeFilter), fileTypeFilter);

        // show the dialog
        result = pFileOpenDialog->Show(nullptr);
        if (result == HRESULT_FROM_WIN32(ERROR_CANCELLED))
            throw std::runtime_error("User didn't choose anything.");

        // get the results
        CComPtr<IShellItemArray> itemArray{};
        pFileOpenDialog->GetResults(&itemArray);

        // how many items did user choose?
        DWORD numOfItems{};
        itemArray->GetCount(&numOfItems);

        // loop through the chosen items
        for (decltype(numOfItems) i = 0; i < numOfItems; ++i) {
            CComPtr<IShellItem> item{};
            itemArray->GetItemAt(i, &item);

            // get file name...
            CComHeapPtr<WCHAR> itemName{};
            item->GetDisplayName(SIGDN_NORMALDISPLAY, &itemName);

            // ... and full file name
            CComHeapPtr<WCHAR> itemFullName{};
            item->GetDisplayName(SIGDN_DESKTOPABSOLUTEEDITING, &itemFullName);

            std::wcout << static_cast<LPWSTR>(itemName) << "\n";
            std::wcout << static_cast<LPWSTR>(itemFullName) << "\n\n";
        }
    }
    catch (std::exception& e) {
        std::cout << "Exception: " << e.what() << "\n";
    }
}
