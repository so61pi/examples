#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <Windows.h>
#include <Shlwapi.h>

#pragma comment(lib, "Shlwapi.lib")


std::wstring LongPathWToShortPathW(const std::wstring& longPath) {
    auto fileAttr = GetFileAttributes(longPath.c_str());
    if (fileAttr == INVALID_FILE_ATTRIBUTES)
        throw std::runtime_error("Path does not exist.");

    // get buffer size to store converted path
    auto length = GetShortPathName(longPath.c_str(), nullptr, 0);
    if (length == 0)
        throw std::runtime_error("Cannot convert path.");

    // convert path
    std::vector<wchar_t> shortPath(length);
    length = GetShortPathName(longPath.c_str(), shortPath.data(), shortPath.size());
    if (length == 0)
        throw std::runtime_error("Cannot convert path.");

    return std::wstring(shortPath.begin(), shortPath.end());
}


std::string LongPathWToShortPathA(const std::wstring& longPath) {
    auto shortPathW = LongPathWToShortPathW(longPath);

    // get required buffer size to store converted path
    std::size_t bufferSize = WideCharToMultiByte(
        CP_UTF8,
        0,
        shortPathW.c_str(),
        shortPathW.size(),
        nullptr,
        0,
        nullptr,
        nullptr);
    if (bufferSize == 0)
        throw std::runtime_error("Cannot convert path.");

    // convert path
    std::vector<char> shortPathA(bufferSize);
    bufferSize = WideCharToMultiByte(
        CP_UTF8,
        0,
        shortPathW.c_str(),
        shortPathW.size(),
        shortPathA.data(),
        shortPathA.size(),
        nullptr,
        nullptr);
    if (bufferSize == 0)
        throw std::runtime_error("Cannot convert path.");

    return std::string(shortPathA.begin(), shortPathA.end());
}


int main() {
    try {
        wchar_t szFile[MAX_PATH]{};

        OPENFILENAME ofn{};
        ofn.lStructSize = sizeof(ofn);
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = L"All Files\0*.*\0";
        ofn.lpstrTitle = L"Check";
        ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
        GetOpenFileName(&ofn);

        std::cout << LongPathWToShortPathA(szFile);
    }
    catch (std::exception& e) {
        std::cout << e.what();
    }
}
