#include <iostream>
#include <string>
#include <vector>

#include <Windows.h>
#include <ntstatus.h>
#include <Winternl.h>

#pragma comment(lib, "ntdll.lib")


int main() {
    ULONG requiredLength{};
    auto status = NtQueryObject(GetCurrentProcess(), ObjectTypeInformation, nullptr, 0, &requiredLength);
    if (status == STATUS_INFO_LENGTH_MISMATCH) {
        std::vector<BYTE> buffer;
        buffer.resize(requiredLength);

        status = NtQueryObject(GetCurrentProcess(), ObjectTypeInformation, buffer.data(), buffer.size(), nullptr);
        if (status == STATUS_SUCCESS) {
            auto poi = *reinterpret_cast<PUBLIC_OBJECT_TYPE_INFORMATION *>(buffer.data());
            std::wstring objectType(poi.TypeName.Buffer, poi.TypeName.Length / sizeof(wchar_t));
            std::wcout << L"Object type : " << objectType;
        }
    }
}
