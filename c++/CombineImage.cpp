#include <array>
#include <cassert>
#include <iostream>
#include <iterator>
#include <numeric>
#include <type_traits>

#include <boost/mpl/vector.hpp>

#define png_infopp_NULL nullptr
#define int_p_NULL      nullptr
#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io/png_dynamic_io.hpp>
#include <boost/gil/extension/dynamic_image/dynamic_image_all.hpp>


namespace gil = boost::gil;
namespace mpl = boost::mpl;


// return the average color from all pixels in the view
template<std::size_t NChannels, typename Pixel>
auto combine_color(Pixel const& p1, Pixel const& p2, double ratio) {
    using std::begin;
    using std::end;

    Pixel combined;
    for (auto i = NChannels; i > 0; --i) {
        combined[i - 1] = p1[i - 1] * ratio + p2[i - 1] * (1 - ratio);
    }

    return combined;
}


template<std::size_t NChannels, typename SrcView, typename DstView>
void combine_image(SrcView const& src1, SrcView const& src2,
                   DstView const& dst, double ratio) {
    static_assert(std::is_same<typename SrcView::const_t,
                               typename DstView::const_t>::value,
                  "must have same view type");

    using std::begin;
    using std::end;

    assert(src1.dimensions() == src2.dimensions());
    assert(src2.dimensions() == dst.dimensions());

    for (auto y = src1.height(); y > 0; --y) {
        for (auto x = src1.width(); x > 0; --x) {
            dst(x - 1, y - 1) =
                combine_color<NChannels>(src1(x - 1, y - 1),
                                         src2(x - 1, y - 1),
                                         ratio);
        }
    }
}


template<typename SrcAnyView>
struct combine_image_visitor {
    using result_type = void;


    combine_image_visitor(SrcAnyView const& src1, SrcAnyView const& src2,
                          double ratio) :
        m_src1{ src1 }, m_src2{ src2 }, m_ratio{ ratio }
    {
        assert(m_src1.dimensions() == m_src2.dimensions());
    }


    auto operator()(gil::rgba8_view_t const& dst) const -> result_type {
        combine_image<4>(m_src1.template _dynamic_cast<gil::rgba8c_view_t>(),
                         m_src2.template _dynamic_cast<gil::rgba8c_view_t>(),
                         dst, m_ratio);
    }


    auto operator()(gil::rgb8_view_t const& dst) const -> result_type {
        combine_image<3>(m_src1.template _dynamic_cast<gil::rgb8c_view_t>(),
                         m_src2.template _dynamic_cast<gil::rgb8c_view_t>(),
                         dst, m_ratio);
    }


    auto operator()(gil::gray8_view_t const& dst) const -> result_type {
        combine_image<1>(m_src1.template _dynamic_cast<gil::gray8c_view_t>(),
                         m_src2.template _dynamic_cast<gil::gray8c_view_t>(),
                         dst, m_ratio);
    }


private:
    SrcAnyView const& m_src1;
    SrcAnyView const& m_src2;
    double m_ratio;
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
void create_output_image(
    typename gil::any_image<ImagesVector>::const_view_t const& in,
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
        image_t in1;
        gil::png_read_image("in1.png", in1);

        image_t in2;
        gil::png_read_image("in2.png", in2);

        if (in1.index() != in2.index()) {
            std::cout << "2 images must have a same format\n";
            return 0;
        }

        if (in1.dimensions() != in2.dimensions()) {
            std::cout << "2 images must have same dimensions\n";
            return 0;
        }

        // create output image
        image_t out;
        create_output_image(gil::const_view(in1), out);

        // combine 2 images
        combine_image_visitor<typename image_t::const_view_t> visitor{
            gil::const_view(in1), gil::const_view(in2), 0.5 };
        gil::apply_operation(gil::view(out), visitor);

        gil::png_write_view("out.png", gil::const_view(out));
    } catch (std::exception const& e) {
        std::cout << e.what() << '\n';
    }
}
