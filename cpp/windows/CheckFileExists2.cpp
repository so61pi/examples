#include <iostream>
#include <string>
#include <Windows.h>
#include <Shlwapi.h>

#pragma comment(lib, "Shlwapi.lib")


bool FileExists(const std::wstring& path) {
    if (PathFileExists(path.c_str()) == FALSE)
        return false;
    return true;
}


int main() {
    wchar_t szFile[MAX_PATH]{};

    OPENFILENAME ofn{};
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = L"All Files\0*.*\0";
    ofn.lpstrTitle = L"Check";
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
    GetOpenFileName(&ofn);

    std::cout << "Does file exist? " << FileExists(szFile) << "\n";
}
