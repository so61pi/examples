#include <cstdio>
#include <exception>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include <boost/multi_array.hpp>
#include <boost/scope_exit.hpp>

#include <zlib.h>
#include <png.h>


template <typename T> using matrix = boost::multi_array<T, 2>;

using file_stream_type = std::fstream;


struct png_ihdr {
    png_uint_32 width    = 0;
    png_uint_32 height   = 0;
    int bit_depth        = 0;
    int color_type       = 0;
    int interlace_type   = 0;
    int compression_type = 0;
    int filter_method    = 0;
};


//
// call back functions
//
void user_error_fn(png_structp, png_const_charp error_msg) {
    throw std::runtime_error(error_msg);
}

void user_warning_fn(png_structp, png_const_charp) {
    // do nothing
}

void read_row_callback(png_structp, png_uint_32 row, int) {
    std::cout << "read row " << row << "\n";
}

void write_row_callback(png_structp, png_uint_32 row, int) {
    std::cout << "write row " << row << "\n";
}


//
// alternative read write functions
//
void PNGAPI read_data_fn(png_structp png_ptr, png_bytep buffer, png_size_t size) {
    auto& fs = *reinterpret_cast<file_stream_type *>(png_get_io_ptr(png_ptr));
    fs.read(reinterpret_cast<file_stream_type::char_type *>(buffer), size);
    // png_error(png_ptr, "Error msg")
}


void PNGAPI write_data_fn(png_structp png_ptr, png_bytep buffer, png_size_t size) {
    auto& fs = *reinterpret_cast<file_stream_type *>(png_get_io_ptr(png_ptr));
    fs.write(reinterpret_cast<file_stream_type::char_type *>(buffer), size);
}


void PNGAPI flush_fn(png_structp png_ptr) {
    auto& fs = *reinterpret_cast<file_stream_type *>(png_get_io_ptr(png_ptr));
    fs.flush();
}


//
// read png to a matrix
//
auto read_png(const std::string& file)
    -> std::tuple<matrix<png_byte>, png_ihdr> {
    file_stream_type fs{ file, std::ios::in | std::ios::binary };

    //
    // read first 8 bytes to check if a file is png or not
    //
    const int num_of_sig_bytes             = 8;
    unsigned char header[num_of_sig_bytes] = {};

    fs.read(reinterpret_cast<file_stream_type::char_type *>(header), num_of_sig_bytes);

    auto is_png = !png_sig_cmp(header, 0, num_of_sig_bytes);
    if (!is_png)
        throw std::runtime_error("File is not PNG.");
    else
        std::cout << "File is PNG.\n";

    //
    // create necessary structs
    //
    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    BOOST_SCOPE_EXIT_ALL(&) {
        if (png_ptr)
            png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    };

    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr,
                                     nullptr);
    if (!png_ptr)
        throw std::runtime_error("Cannot create png_struct.");

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
        throw std::runtime_error("Cannot create png_info.");

    //
    // set up error & warning handler
    //

    // the default error handler is unsafe in C++ because it uses setjmp &
    // longjmp to handle errors
    png_set_error_fn(png_ptr, nullptr,
                     user_error_fn,  // if null, use default error handler with
                                     // setjmp & longjmp!!!
                     user_warning_fn // if null, use default warning handler
                     );

    //
    // init
    //
    png_set_read_fn(png_ptr, reinterpret_cast<png_voidp>(&fs), read_data_fn);
    png_set_sig_bytes(png_ptr, num_of_sig_bytes);

    png_set_read_status_fn(
        png_ptr,
        read_row_callback // will be called when a row is read
        );
        
    //
    // read header
    //
    png_read_info(png_ptr, info_ptr);

    png_ihdr ihdr;

    png_get_IHDR(png_ptr, info_ptr, &ihdr.width, &ihdr.height, &ihdr.bit_depth,
                 &ihdr.color_type, &ihdr.interlace_type, &ihdr.compression_type,
                 &ihdr.filter_method);

    // auto width = png_get_image_width(png_ptr, info_ptr);
    // auto height = png_get_image_height(png_ptr, info_ptr);
    // auto bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    // auto color_type = png_get_color_type(png_ptr, info_ptr);
    // auto interlace_type = png_get_interlace_type(png_ptr, info_ptr);
    // auto compression_type = png_get_compression_type(png_ptr, info_ptr);
    // auto filter_method = png_get_filter_type(png_ptr, info_ptr);
    // auto channels = png_get_channels(png_ptr, info_ptr);
    // auto rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    // auto signature = png_get_signature(png_ptr, info_ptr);

    auto rowbytes = png_get_rowbytes(png_ptr, info_ptr);

    //
    // read image
    //

    // write a whole image
    matrix<png_byte> image;
    image.resize(boost::extents[ihdr.height][rowbytes]);
    
    std::vector<png_bytep> row_ptrs;
    
    for (auto&& row : image) {
        row_ptrs.push_back(row.origin());
    }

    png_read_image(png_ptr, row_ptrs.data());

    // read several rows
    // int number_of_rows = 1;
    // png_read_rows(png_ptr, row_ptrs.data(), nullptr, number_of_rows);

    // read one row
    // png_bytep row_ptr = nullptr;
    // png_read_row(png_ptr, row_ptr, nullptr);

    // finish reading
    png_read_end(png_ptr, info_ptr);

    return std::make_tuple(std::move(image), std::move(ihdr));
}


//
// write png to file
//
void write_png(const std::string& file,
               std::tuple<matrix<png_byte>, png_ihdr>& data) {
    auto& image = std::get<matrix<png_byte>>(data);
    auto& ihdr  = std::get<png_ihdr>(data);

    //
    // open file for writing
    //
    file_stream_type fs{ file, std::ios::out | std::ios::binary };

    //
    // create necessary structs
    //
    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    BOOST_SCOPE_EXIT_ALL(&) {
        if (png_ptr)
            png_destroy_write_struct(&png_ptr, &info_ptr);
    };

    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr,
                                      nullptr);
    if (!png_ptr)
        throw std::runtime_error("Cannot create png_struct.");

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
        throw std::runtime_error("Cannot create png_info.");

    //
    // set up error & warning handler
    //

    // the default error handler is unsafe in C++ because it uses setjmp &
    // longjmp to handle errors
    png_set_error_fn(png_ptr, nullptr,
                     user_error_fn,  // if null, use default error handler with
                                     // setjmp & longjmp!!!
                     user_warning_fn // if null, use default warning handler
                     );

    //
    // init
    //
    png_set_write_fn(png_ptr, reinterpret_cast<png_voidp>(&fs), write_data_fn,
                     flush_fn);

    // set write callback
    png_set_write_status_fn(
        png_ptr,
        write_row_callback // will be called when a row is written
        );

    // set compress level
    png_set_compression_level(png_ptr, Z_BEST_COMPRESSION);

    //
    // write header
    //
    png_set_IHDR(png_ptr, info_ptr, ihdr.width, ihdr.height, ihdr.bit_depth,
                 ihdr.color_type, ihdr.interlace_type, ihdr.compression_type,
                 ihdr.filter_method);

    png_write_info(png_ptr, info_ptr);

    //
    // write image
    //

    // write a whole image
    std::vector<png_bytep> row_ptrs;
    for (auto&& row : image) {
        row_ptrs.push_back(row.origin());
    }

    png_write_image(png_ptr, row_ptrs.data());

    // write several rows
    // int number_of_rows = 1;
    // png_write_rows(png_ptr, row_ptrs.data(), number_of_rows);

    // write a single row
    // png_bytep row_ptr = nullptr;
    // png_write_row(png_ptr, row_ptr);

    // finish writing
    png_write_end(png_ptr, info_ptr);
}


int main() {
    try {
        auto data = read_png("in.png");
        write_png("out.png", data);
    } catch (std::exception const& e) {
        std::cout << "exception : " << e.what() << "\n";
    }
}
