#include <exception>
#include <iostream>
#include <stdexcept>
#include <vector>

#include <boost/scope_exit.hpp>

#define png_infopp_NULL nullptr
#define int_p_NULL nullptr
#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io/png_io.hpp>

#include <Windows.h>


namespace gil = boost::gil;


using image_t   = gil::bgr8_image_t;
using pixel_t   = image_t::value_type;
using channel_t = gil::channel_type<image_t>::type;


auto TakeWindowShot(HWND hWnd) -> image_t {
    std::runtime_error exception{"cannot take window screen shot"};

    RECT windowSize{};
    if (GetWindowRect(hWnd, &windowSize) == FALSE)
        throw exception;

    auto windowWidth  = windowSize.right - windowSize.left;
    auto windowHeight = windowSize.bottom - windowSize.top;

    HDC hDC         = nullptr;
    HDC hMemoryDC   = nullptr;
    HBITMAP hBitmap = nullptr;

    BOOST_SCOPE_EXIT_ALL(&) {
        if (hBitmap) {
            DeleteObject(hBitmap);
        }
        if (hMemoryDC) {
            DeleteObject(hMemoryDC);
        }
        if (hDC) {
            ReleaseDC(hWnd, hDC);
        }
    };

    hDC = GetDC(hWnd);
    if (!hDC) {
        throw exception;
    }

    hMemoryDC = CreateCompatibleDC(hDC);
    if (!hMemoryDC) {
        throw exception;
    }

    hBitmap = CreateCompatibleBitmap(hDC, windowWidth, windowHeight);
    if (!hBitmap) {
        throw exception;
    }

    auto soResult = SelectObject(hMemoryDC, hBitmap);
    if (soResult == HGDI_ERROR || soResult == nullptr) {
        throw exception;
    }

    if (BitBlt(hMemoryDC, 0, 0, windowWidth, windowHeight, hDC, 0, 0,
               SRCCOPY | CAPTUREBLT)
        == FALSE) {
        throw exception;
    }

    BITMAP bitmap{};
    if (GetObject(hBitmap, sizeof(bitmap), &bitmap) == 0) {
        throw exception;
    }

    // use bottom-up view by default
    BITMAPINFOHEADER bi{};
    bi.biSize          = sizeof(bi);
    bi.biWidth         = bitmap.bmWidth;
    bi.biHeight        = bitmap.bmHeight;
    bi.biPlanes        = 1;
    bi.biBitCount      = 24;
    bi.biCompression   = BI_RGB;
    bi.biSizeImage     = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed       = 0;
    bi.biClrImportant  = 0;

    auto bpp     = bi.biBitCount / 8;
    auto rowsize = bi.biWidth * bpp;
    auto imgsize = rowsize * bi.biHeight;

    std::vector<channel_t> pixels;
    pixels.resize(imgsize);

    auto gdibResult =
        GetDIBits(hDC, hBitmap, 0, bi.biHeight,
                  reinterpret_cast<LPVOID>(pixels.data()),
                  reinterpret_cast<LPBITMAPINFO>(&bi), DIB_RGB_COLORS);
    if (gdibResult == ERROR_INVALID_PARAMETER || gdibResult == 0) {
        throw exception;
    }

    auto bottomUpView =
        gil::interleaved_view(bi.biWidth, bi.biHeight,
                              reinterpret_cast<pixel_t const*>(pixels.data()),
                              rowsize);
    auto topDownView = gil::flipped_up_down_view(bottomUpView);
    image_t screen{topDownView.width(), topDownView.height()};
    gil::copy_pixels(topDownView, gil::view(screen));
    return screen;
}


int main() {
    try {
        auto screen = TakeWindowShot(GetDesktopWindow());
        gil::png_write_view("screen.png", boost::gil::const_view(screen));
    } catch (std::exception const& e) {
        std::cout << e.what() << '\n';
    } catch (...) {
        std::cout << "except\n";
    }
}
