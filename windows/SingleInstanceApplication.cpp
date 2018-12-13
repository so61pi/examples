#include <Windows.h>


int main() {
    // create a global mutex
    auto mutex = CreateMutex(nullptr, TRUE, L"Global\\SingleInstanceApplication");
    if (mutex == nullptr) {
        MessageBox(nullptr, L"Cannot create mutex.", L"Message", MB_ICONERROR);
        return 0;
    }
    else {
        // if it's already created, that means this program is already run
        // then exit
        if (GetLastError() == ERROR_ALREADY_EXISTS) {
            MessageBox(
                nullptr,
                L"Another instance of this program is already running.",
                L"Message",
                MB_ICONEXCLAMATION);

            ReleaseMutex(mutex);
            return 0;
        }
    }

    MessageBox(nullptr, L"Running.", L"Message", MB_ICONINFORMATION);

    ReleaseMutex(mutex);
}
