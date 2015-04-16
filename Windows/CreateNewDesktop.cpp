#include <iostream>
#include <Windows.h>


int main() {
    auto desktopName = L"NewDesktop";

    // create new desktop
    auto hNewDesktop = CreateDesktop(desktopName, nullptr, nullptr, 0, GENERIC_ALL, nullptr);
    if (hNewDesktop != nullptr) {
        //
        // switch to new desktop
        //

        // display new desktop
        SwitchDesktop(hNewDesktop);

        // change desktop for this thread to new desktop
        SetThreadDesktop(hNewDesktop);


        //
        // do thing
        //

        // show message box in new desktop
        MessageBox(nullptr, L"Notepad will run now.", L"Caption", MB_ICONINFORMATION);

        // run Notepad in new desktop
        PROCESS_INFORMATION pi{};
        STARTUPINFO si{};
        si.cb = sizeof(si);
        si.lpDesktop = const_cast<LPWSTR>(desktopName);
        auto result = CreateProcess(
            L"C:\\Windows\\system32\\Notepad.exe",
            nullptr,    // no command line
            nullptr,    // use default security descriptor
            nullptr,    // use default security descriptor
            FALSE,      // no handle inheritance
            0,          // no creation flags
            nullptr,    // use the environment of calling process
            nullptr,    // use calling process' current directory
            &si,        // specify desktop
            &pi);
        if (result != 0) {
            // wait until Notepad exits
            WaitForSingleObject(pi.hProcess, INFINITE);

            CloseHandle(pi.hThread);
            CloseHandle(pi.hProcess);
        }
        else {
            std::cout << "Cannot create new process: " << GetLastError() << "\n";
        }
        

        //
        // switch back to Default desktop
        //
        
        auto hDefaultDesktop = OpenDesktop(L"Default", 0, FALSE, GENERIC_ALL);
        
        // change desktop for this thread back
        SetThreadDesktop(hDefaultDesktop);

        // display Default desktop
        SwitchDesktop(hDefaultDesktop);

        // close after using
        CloseDesktop(hDefaultDesktop);

        CloseDesktop(hNewDesktop);
    }
    else {
        std::cout << "Cannot create new desktop: " << GetLastError() << "\n";
    }
}
