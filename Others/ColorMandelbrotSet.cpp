#include <algorithm>
#include <cinttypes>
#include <cmath>
#include <complex>
#include <iostream>
#include <iterator>
#include <vector>

#define png_infopp_NULL nullptr
#define int_p_NULL      nullptr
#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io/png_io.hpp>


namespace gil = boost::gil;


// map value in range r1 to r2
auto map_value(double val, double r1_from, double r1_to, double r2_from, double r2_to) -> double {
    return ((val - r1_from) / (r1_to - r1_from)) * (r2_to - r2_from) + r2_from;
}


class mandelbrot_fn {
    struct color_line {
        explicit color_line(int lv) : 
            level{ lv }
        {}


        color_line(int lv, gil::bits8 r, gil::bits8 g, gil::bits8 b) :
            level{ lv },
            pixel{ r, g, b }
        {}


        int                 level;
        gil::rgb8_pixel_t   pixel;


        auto operator<(color_line const& rhs) const noexcept -> bool {
            return level < rhs.level;
        }
    };


public:
    using const_t           = mandelbrot_fn;
    using value_type        = gil::rgb8_pixel_t;
    using reference         = value_type;
    using const_reference   = value_type;
    using point_t           = gil::point2<int>;
    using result_type       = value_type;
    using argument_type     = point_t;
    static constexpr bool is_mutable = false;


    explicit mandelbrot_fn(point_t const& size) :
        m_size{ size }
    {
        m_colors.emplace_back( 0,   0,   0,  64);
        m_colors.emplace_back( 8,   0, 255, 255);
        m_colors.emplace_back(16,   0, 255,   0);
        m_colors.emplace_back(24, 255, 128,   0);
        m_colors.emplace_back(32,   0,   0, 255);
        m_colors.emplace_back(40, 255, 255,   0);
        m_colors.emplace_back(48, 255, 255, 255);
        m_colors.emplace_back(56, 255, 255, 255);
        m_colors.emplace_back(64, 255, 255, 255);
        sort(begin(m_colors), end(m_colors));
    }


    auto operator()(point_t const& point) const -> result_type {
        auto level = get_color_level(point);
        if (level < 0) {
            return result_type{ 0, 0, 0 };
        }

        auto uppos = upper_bound(begin(m_colors), end(m_colors), color_line{ level });
        if (uppos == end(m_colors)) {
            --uppos;
        }

        auto const& lower = *(uppos - 1);
        auto const& upper = *uppos;
        result_type pixel;
        for (auto i = 0; i < 3; ++i) {
            pixel[i] = map_value(level, lower.level, upper.level,
                                 lower.pixel[i], upper.pixel[i]);
        }
        return pixel;
    }


private:
    // get color level of given point
    // negative result if the point is in the set
    auto get_color_level(point_t const& point) const -> int {
        // map x to [-2, 1] and y to [1.5, -1.5]
        std::complex<double> c{map_value(point.x, 0, m_size.x, -2, 1),
                               map_value(point.y, 0, m_size.y, 1.5, -1.5)};
        int level = 0;
        for (auto lc = c; level < 64; ++level) {
            if (std::pow(lc.real(), 2) + std::pow(lc.imag(), 2) > 4) {
                return level;
            }
            lc = std::pow(lc, 2) + c;
        }
        return -1;
    }


    point_t m_size;
    std::vector<color_line> m_colors;
};


int main() {
    using point_t = mandelbrot_fn::point_t;
    using locator_t = gil::virtual_2d_locator<mandelbrot_fn, false>;
    using image_view_t = gil::image_view<locator_t>;

    point_t size{ 20'000, 20'000 };
    image_view_t view{ size, locator_t{ point_t{ 0, 0 }, point_t{ 1, 1 }, mandelbrot_fn{ size } } };
    gil::png_write_view("mandelbrot.png", view);
}
