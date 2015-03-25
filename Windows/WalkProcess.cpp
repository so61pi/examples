#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>
#include <Psapi.h>


std::wstring FormatError(DWORD error) {
    wchar_t szErrorMessage[200]{};
    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, error, LANG_SYSTEM_DEFAULT, szErrorMessage, sizeof(szErrorMessage), nullptr);
    return std::wstring(szErrorMessage);
}


int main() {
    std::vector<DWORD> processIds;
    processIds.resize(256);
    DWORD bytesReturned = 0;
    auto result = EnumProcesses(processIds.data(), processIds.size() * sizeof(DWORD), &bytesReturned);
    if (result != 0) {
        processIds.resize(bytesReturned / sizeof(DWORD));
        for (auto pid : processIds) {
            auto hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
            if (hProcess != nullptr) {
                // get process' full file name
                wchar_t fileName[MAX_PATH]{};
                DWORD size = MAX_PATH;
                QueryFullProcessImageName(hProcess, 0, fileName, &size);

                std::wcout << fileName << "\n";
            }
            else {
                std::wcout << FormatError(GetLastError());
            }
        }
    }
}
