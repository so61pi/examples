#include <iostream>
#include <string>
#include <Windows.h>


bool FileExists(const std::wstring& path) {
    auto fileAttr = GetFileAttributes(path.c_str());
    if (fileAttr == INVALID_FILE_ATTRIBUTES || fileAttr & FILE_ATTRIBUTE_DIRECTORY)
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
