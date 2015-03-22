#include <iostream>
#include <Windows.h>


int main() {
    // get current desktop wallpaper path
    wchar_t szWallpaperPath[MAX_PATH]{};
    auto result = SystemParametersInfo(SPI_GETDESKWALLPAPER, sizeof(szWallpaperPath), szWallpaperPath, 0);
    if (result == FALSE) {
        MessageBox(NULL, L"Cannot get current desktop wallpaper path.", L"Message", MB_ICONINFORMATION);
        return -1;
    }

    std::wcout << szWallpaperPath << "\n";

    // change desktop wallpaper
    result = SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, szWallpaperPath, SPIF_UPDATEINIFILE);
    if (result == FALSE) {
        MessageBox(NULL, L"Cannot change desktop wallpaper.", L"Message", MB_ICONINFORMATION);
        return -1;
    }
}
