#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <boost/scope_exit.hpp>
#include <Windows.h>
#include <gdiplus.h>

#pragma comment(lib, "gdiplus.lib")


bool FindImageEncoder(const std::wstring& mimeType, CLSID& clsid);
void SaveImage(const std::wstring& fileName, HBITMAP hBitmap, const std::wstring& mimeType = L"image/png");
void TakeScreenShot(HWND hWnd, const std::wstring& fileName);


//
// find a codec from the given mime type
//
bool FindImageEncoder(const std::wstring& mimeType, CLSID& clsid) {
    UINT num{}; // number of image encoders
    UINT size{}; // size of image encoder array in bytes

    // get the number of available encoders
    if (Gdiplus::GetImageEncodersSize(&num, &size) != Gdiplus::Status::Ok)
        throw std::runtime_error{ "Cannot get image encoder." };

    // if there's no codec, return false
    if (!num || !size)
        return false;

    // get all codecs' information
    // because the num and size returned by GetImageEncodersSize
    // doesn't match with each other (num != size * sizeof(ImageCodecInfo)),
    // therefore the number of available codecs must be determined by size
    std::vector<Gdiplus::ImageCodecInfo> imageCodecInfos(size / sizeof(Gdiplus::ImageCodecInfo) + 1);
    if (Gdiplus::GetImageEncoders(
            num,
            size,
            imageCodecInfos.data()) != Gdiplus::Status::Ok)
        throw std::runtime_error{ "Cannot get image encoder." };

    // find appropriate codec
    for (auto& codec : imageCodecInfos) {
        if (codec.MimeType == mimeType) {
            clsid = codec.Clsid;
            return true;
        }
    }

    return false;
}


//
// save image to file
//
void SaveImage(const std::wstring& fileName, HBITMAP hBitmap, const std::wstring& mimeType) {
    Gdiplus::Bitmap bm{ hBitmap, nullptr };
    CLSID clsid{};
    if (FindImageEncoder(mimeType, clsid)) {
        if (bm.Save(fileName.c_str(), &clsid) != Gdiplus::Status::Ok)
            throw std::runtime_error{ "Cannot save image." };
    }
    else
        throw std::runtime_error{ "Cannot save image." };
}


//
// take a screen shot of given window and save to file
//
void TakeScreenShot(HWND hWnd, const std::wstring& fileName) {
    if (!hWnd)
        throw std::runtime_error{ "Invalid window handle." };

    RECT windowSize{};
    if (GetWindowRect(hWnd, &windowSize) == FALSE)
        throw std::runtime_error{ "Cannot take window screen shot." };

    auto windowWidth = windowSize.right - windowSize.left;
    auto windowHeight = windowSize.bottom - windowSize.top;

    // get device context from the given window
    auto hDC = GetDC(hWnd);
    if (!hDC)
        throw std::runtime_error{ "Cannot take window screen shot." };

    // create memory DC
    auto hMemoryDC = CreateCompatibleDC(hDC);
    if (!hMemoryDC)
        throw std::runtime_error{ "Cannot take window screen shot." };

    // create bitmap
    auto hBitmap = CreateCompatibleBitmap(hDC, windowWidth, windowHeight);
    if (!hBitmap)
        throw std::runtime_error{ "Cannot take window screen shot." };

    // select bitmap to created memory DC
    auto result = SelectObject(hMemoryDC, hBitmap);
    if (result == HGDI_ERROR || result == nullptr)
        throw std::runtime_error{ "Cannot take window screen shot." };

    // transfer block of data to memory DC
    if (!BitBlt(
        hMemoryDC, 0, 0, windowWidth, windowHeight,
        hDC, 0, 0,
        SRCCOPY | CAPTUREBLT))
        throw std::runtime_error{ "Cannot take window screen shot." };

    // save image to file
    SaveImage(fileName, hBitmap);

    // release resources
    BOOST_SCOPE_EXIT_ALL(&) {
        if (hDC)
            ReleaseDC(hWnd, hDC);

        if (hMemoryDC)
            DeleteDC(hMemoryDC);

        if (hBitmap)
            DeleteObject(hBitmap);
    };
}


int main() {
    Gdiplus::GdiplusStartupInput gdiplusStartupInput{};
    ULONG_PTR gdiplusToken{};

    try {
        if (Gdiplus::GdiplusStartup(
                &gdiplusToken,
                &gdiplusStartupInput,
                nullptr) != Gdiplus::Status::Ok)
            throw std::runtime_error{ "Cannot start GDI+." };

        TakeScreenShot(GetDesktopWindow(), L"D:\\screen.png");

        Gdiplus::GdiplusShutdown(gdiplusToken);
    }
    catch (std::exception& e) {
        std::cout << e.what() << "\n";
    }
}
