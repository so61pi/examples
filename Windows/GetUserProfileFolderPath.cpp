#include <iostream>
#include <Windows.h>
#include <ShlObj.h>


int main() {
    wchar_t *szPath{};
    auto hr = SHGetKnownFolderPath(FOLDERID_Profile, KF_FLAG_DEFAULT_PATH, NULL, &szPath);
    if (hr != S_OK) {
        MessageBox(NULL, L"Cannot get folder path.", L"Message", MB_ICONINFORMATION);
        return -1;
    }

    std::wcout << szPath << "\n";

    CoTaskMemFree((LPVOID)szPath);
}
