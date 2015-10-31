#include <array>
#include <cassert>
#include <iostream>
#include <iterator>
#include <numeric>

#include <boost/mpl/vector.hpp>

#define png_infopp_NULL nullptr
#define int_p_NULL      nullptr
#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io/png_dynamic_io.hpp>
#include <boost/gil/extension/dynamic_image/dynamic_image_all.hpp>


namespace gil = boost::gil;
namespace mpl = boost::mpl;


// return the average color from all pixels in the view
template<std::size_t NChannels, typename View>
auto average_color(View const& view) {
    using std::begin;
    using std::end;

    using pixel_t = typename View::value_type;
    using channel_t = typename gil::channel_type<View>::type;

    auto accumulator = [](std::array<int, NChannels> sum, pixel_t const& val) {
        for (auto i = NChannels; i > 0; --i) { sum[i - 1] += val[i - 1]; }
        return sum;
    };

    auto sum = std::accumulate(begin(view), end(view),
        std::array<int, NChannels>{}, accumulator);

    pixel_t avg;
    for (auto i = NChannels; i > 0; --i) {
        avg[i - 1] = static_cast<channel_t>(sum[i - 1] / view.size());
    }

    return avg;
}


template<typename SrcView>
auto create_roi_view(SrcView const& src,
                     typename SrcView::x_coord_t x,
                     typename SrcView::y_coord_t y,
                     int size = 3) {
    assert(size > 0);

    // [x_min; x_max)
    auto x_min = x - (size - 1) / 2;
    auto x_max = x_min + size;
    if (x_min < 0) x_min = 0;
    if (x_max > src.width()) x_max = src.width();

    // [y_min; y_max)
    auto y_min = y - (size - 1) / 2;
    auto y_max = y_min + size;
    if (y_min < 0) y_min = 0;
    if (y_max > src.height()) y_max = src.height();

    return gil::subimage_view(src, x_min, y_min, x_max - x_min, y_max - y_min);
}


template<std::size_t NChannels, typename SrcView, typename DstView>
void blur_image(SrcView const& src, DstView const& dst) {
    using std::begin;
    using std::end;

    assert(src.dimensions() == dst.dimensions());

    for (auto y = src.height(); y > 0; --y) {
        for (auto x = src.width(); x > 0; --x) {
            auto roi_view = create_roi_view(src, x - 1, y - 1);
            dst(x - 1, y - 1) = average_color<NChannels>(roi_view);
        }
    }
}


template<typename DstAnyView>
struct blur_visitor {
    using result_type = void;


    blur_visitor(DstAnyView const& dst) :
        m_dst{ dst }
    {}


    // each image type has a different number of channels
#define CALL_OPERATOR(NChannels, SrcView, DstView)              \
    auto operator()(SrcView const& src) const -> result_type {  \
        blur_image<NChannels>(src,                              \
            m_dst.template _dynamic_cast<DstView>());           \
    }

    CALL_OPERATOR(4, gil::rgba8c_view_t, gil::rgba8_view_t)
    CALL_OPERATOR(3, gil::rgb8c_view_t, gil::rgb8_view_t)
    CALL_OPERATOR(1, gil::gray8c_view_t, gil::gray8_view_t)


#undef CALL_OPERATOR


private:
    DstAnyView const& m_dst;
};


// loop through a types sequence until function return true
template<std::size_t Current, std::size_t End>
struct for_until_position_helper {
    template<typename UnaryFunction>
    auto operator()(UnaryFunction f) const -> std::size_t {
        if (f(std::integral_constant<std::size_t, Current>{}))
            return Current;
        return for_until_position_helper<Current + 1, End>{}(f);
    }
};

template<std::size_t End>
struct for_until_position_helper<End, End> {
    template<typename UnaryFunction>
    auto operator()(UnaryFunction) const -> std::size_t {
        return End;
    }
};

template<typename Sequence, typename UnaryFunction>
auto for_until_position(UnaryFunction f) -> std::size_t {
    return for_until_position_helper<0, mpl::size<Sequence>::value>{}(f);
}


// create output image bases on underlying type of input image
template<typename ImagesVector>
void create_output_image(typename gil::any_image<ImagesVector>::const_view_t const& in,
                         gil::any_image<ImagesVector>& out) {
    for_until_position<ImagesVector>(
        [&](auto index_carrier) -> bool {
            if (index_carrier() == in.index()) {
                using real_image_t =
                    typename mpl::at_c<ImagesVector, index_carrier()>::type;
                out = real_image_t{ in.dimensions() };
                return true;
            }
            return false;
        });
}


int main() {
    try {
        using images_vector_t = mpl::vector<gil::rgba8_image_t,
                                            gil::rgb8_image_t,
                                            gil::gray8_image_t>;
        using image_t = gil::any_image<images_vector_t>;

        // read image to view
        image_t in;
        gil::png_read_image("in.png", in);
        auto view = gil::const_view(in);

        // create output image
        image_t out;
        create_output_image(view, out);

        // blurring
        blur_visitor<typename image_t::view_t> visitor{ gil::view(out) };
        gil::apply_operation(view, visitor);
        gil::png_write_view("out.png", gil::const_view(out));
    } catch (std::exception const& e) {
        std::cout << e.what() << '\n';
    }
}
