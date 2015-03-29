#include <iostream>
#include <Windows.h>
#include <VersionHelpers.h>


int main() {
    if (!IsWindowsServer())
        std::cout << "You need a Windows Server release.\n";

    if (!IsWindows7OrGreater())
        std::cout << "You need at least Windows 7.\n";
    
    /*
    Deprecated from Windows 8.1

    OSVERSIONINFOEX osinfo{};
    osinfo.dwOSVersionInfoSize = sizeof(osinfo);
    GetVersionEx(reinterpret_cast<OSVERSIONINFO *>(&osinfo));
    std::cout << "Major version: " << osinfo.dwMajorVersion
        << "\nMinor version: " << osinfo.dwMinorVersion
        << "\nBuild number: " << osinfo.dwBuildNumber
        << "\nPlatform id: " << osinfo.dwPlatformId
        << "\nCSD Version: " << osinfo.szCSDVersion
        << "\n";
    */
}
