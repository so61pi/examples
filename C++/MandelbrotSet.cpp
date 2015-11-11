#include <cinttypes>
#include <cmath>
#include <complex>
#include <iostream>

#define png_infopp_NULL nullptr
#define int_p_NULL      nullptr
#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io/png_io.hpp>


namespace gil = boost::gil;


struct mandelbrot_fn {
    using const_t           = mandelbrot_fn;
    using value_type        = gil::gray8_pixel_t;
    using reference         = value_type;
    using const_reference   = value_type;
    using point_t           = gil::point2<int>;
    using result_type       = value_type;
    using argument_type     = point_t;
    static constexpr bool is_mutable = false;

    explicit mandelbrot_fn(point_t const& size) :
        m_size{ size }
    {}

    auto operator()(point_t const& p) const -> result_type {
        auto map = [](double val, double r1_from, double r1_to, double r2_from, double r2_to) -> double {
            return ((val - r1_from) / (r1_to - r1_from)) * (r2_to - r2_from) + r2_from;
        };

        // map x to [-2, 1] and y to [1.5, -1.5]
        std::complex<double> c{ map(p.x, 0, m_size.x, -2, 1), map(p.y, 0, m_size.y, 1.5, -1.5) };
        auto lc = c;
        for (auto i = 0; i < 100; ++i) {
            lc = std::pow(lc, 2) + c;
            if (std::pow(lc.real(), 2) + std::pow(lc.imag(), 2) > 4) {
                return result_type{ static_cast<gil::bits8>(i / 100.0 * 255) };
            }
        }

        return result_type{ 0 };
    }

private:
    point_t m_size;
};


int main() {
    using point_t = mandelbrot_fn::point_t;
    using locator_t = gil::virtual_2d_locator<mandelbrot_fn, false>;
    using image_view_t = gil::image_view<locator_t>;

    point_t size{ 5000, 5000 };
    image_view_t view{ size, locator_t{ point_t{ 0, 0 }, point_t{ 1, 1 }, mandelbrot_fn{ size } } };
    gil::png_write_view("mandelbrot.png", view);
}
