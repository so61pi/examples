#include <iostream>
#include <Windows.h>
#include <atlbase.h>
#include <Shobjidl.h>


int main() {
    class tagCoInit {
        HRESULT m_result;

    public:
        tagCoInit() {
            m_result = CoInitialize(nullptr);
        }

        ~tagCoInit() {
            if (IsSucceeded())
                CoUninitialize();
        }

        bool IsSucceeded() const {
            return SUCCEEDED(m_result);
        }
    } CoInit;

    if (CoInit.IsSucceeded()) {
        CComPtr<IDesktopWallpaper> pDW;
        auto result = pDW.CoCreateInstance(__uuidof(DesktopWallpaper));
        if (SUCCEEDED(result)) {
            // check if all monitors are using a same wallpaper
            // or there is a slideshow running
            CComHeapPtr<wchar_t> pszWallpaperPath;
            result = pDW->GetWallpaper(nullptr, &pszWallpaperPath);
            if (SUCCEEDED(result)) {
                // all monitors have a same wallpaper
                if (result == S_OK) {
                    MessageBox(nullptr, pszWallpaperPath, L"Message", MB_ICONINFORMATION);
                }
                // each monitor has its own wallpaper or a slideshow is running
                else {
                    // get number of monitors
                    UINT uNumOfMonitors = 0;
                    result = pDW->GetMonitorDevicePathCount(&uNumOfMonitors);
                    if (SUCCEEDED(result)) {
                        if (uNumOfMonitors != 0) {
                            // get current wallpaper at each monitor
                            for (auto i = 0u; i < uNumOfMonitors; ++i) {
                                CComHeapPtr<wchar_t> pszMonitorPath;
                                result = pDW->GetMonitorDevicePathAt(i, &pszMonitorPath);
                                if (SUCCEEDED(result)) {
                                    CComHeapPtr<wchar_t> pszWallpaperPath;
                                    result = pDW->GetWallpaper(pszMonitorPath, &pszWallpaperPath);
                                    if (SUCCEEDED(result)) {
                                        MessageBox(nullptr, pszWallpaperPath, L"Message", MB_ICONINFORMATION);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
