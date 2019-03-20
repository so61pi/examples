#include <Windows.h>


int main() {
    const auto hKey = HKEY_CURRENT_USER;
    const auto key = L"Software\\so61pi";
    const auto value = L"Test";
    const auto dataType = REG_DWORD;

    // set value
    DWORD setData = 314;
    DWORD dataSize = sizeof(setData);
    auto result = RegSetKeyValue(hKey, key, value, dataType, &setData, dataSize);
    if (result != ERROR_SUCCESS) {
        MessageBox(nullptr, L"Cannot set value to registry.", L"Message", MB_ICONINFORMATION);
        return -1;
    }

    // get value
    auto requiredType = RRF_RT_REG_DWORD;
    DWORD retrievedType = 0;
    DWORD retrievedData = 0;
    dataSize = sizeof(retrievedData);
    result = RegGetValue(hKey, key, value, requiredType, &retrievedType, &retrievedData, &dataSize);
    if (result != ERROR_SUCCESS) {
        MessageBox(nullptr, L"Cannot get specified value from registry.", L"Message", MB_ICONINFORMATION);
        return -1;
    }
}
