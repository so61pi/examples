#include <iostream>
#include <Windows.h>
#include <strsafe.h>


int main() {
    wchar_t szFile[MAX_PATH]{};

    OPENFILENAME ofn{};
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = L"All Files\0*.*\0";
    ofn.lpstrTitle = L"Check";
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
    if (GetOpenFileName(&ofn) != FALSE) {
        std::size_t length{};
        StringCbLength(szFile, MAX_PATH, &length);
    }
}
