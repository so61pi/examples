/*
This file can be compiled successfully but cannot run.
Please read the comments in read_png & write_png functions to understand how to use libpng.
*/

#include <cstdio>
#include <exception>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include <boost/scope_exit.hpp>

#include <zlib.h>
#include <png.h>


void user_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    throw std::runtime_error(error_msg);
}


void user_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    // do nothing
}


void read_row_callback(png_structp png_ptr, png_uint_32 row, int pass) {
    std::cout << "read row " << row << "\n";
}


void write_row_callback(png_structp png_ptr, png_uint_32 row, int pass) {
    std::cout << "write row " << row << "\n";
}


void read_png(const std::string& file) {
    FILE *fp = nullptr;
    BOOST_SCOPE_EXIT_ALL(&) {
        if (fp) fclose(fp);
    };


    //
    // open file for reading
    //
    fp = fopen(file.c_str(), "rb");
    if (!fp)
        throw std::runtime_error("Cannot open file.");


    //
    // read first 8 bytes to check if a file is png or not
    //
    const int num_of_sig_bytes = 8;
    unsigned char header[num_of_sig_bytes] = {};

    fread(header, 1, num_of_sig_bytes, fp);

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
        if (png_ptr) png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    };

    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr)
        throw std::runtime_error("Cannot create png_struct.");

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
        throw std::runtime_error("Cannot create png_info.");


    //
    // set up error & warning handler
    //

    // the default error handler is unsafe in C++ because it uses setjmp & longjmp to handle errors
    png_set_error_fn(
        png_ptr,
        nullptr,
        user_error_fn, /* if null, use default error handler with setjmp & longjmp!!! */
        user_warning_fn /* if null, use default warning handler */);


    //
    // init
    //
    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, num_of_sig_bytes);

    png_set_read_status_fn(
        png_ptr,
        read_row_callback /* will be called when a row is read */);


    //
    // read header
    //
    png_read_info(png_ptr, info_ptr);

    png_uint_32 width = 0;
    png_uint_32 height = 0;
    int bit_depth = 0;
    int color_type = 0;
    int interlace_type = 0;
    int compression_type = 0;
    int filter_method = 0;
    png_get_IHDR(
        png_ptr,
        info_ptr,
        &width,
        &height,
        &bit_depth,
        &color_type,
        &interlace_type,
        &compression_type,
        &filter_method);

    /*
    auto width = png_get_image_width(png_ptr, info_ptr);
    auto height = png_get_image_height(png_ptr, info_ptr);
    auto bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    auto color_type = png_get_color_type(png_ptr, info_ptr);
    auto interlace_type = png_get_interlace_type(png_ptr, info_ptr);
    auto compression_type = png_get_compression_type(png_ptr, info_ptr);
    auto filter_method = png_get_filter_type(png_ptr, info_ptr);
    auto channels = png_get_channels(png_ptr, info_ptr);
    auto rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    auto signature = png_get_signature(png_ptr, info_ptr);
    */


    //
    // read image
    //

    // read a whole image
    std::vector<png_bytep> row_pointers(height);
    png_read_image(png_ptr, row_pointers.data());

    // read several rows
    int number_of_rows = 1;
    png_read_rows(png_ptr, row_pointers.data(), nullptr, number_of_rows);

    // read one row
    png_bytep row_pointer = nullptr;
    png_read_row(png_ptr, row_pointer, nullptr);

    // finish reading
    png_read_end(png_ptr, nullptr);
}


void write_png(const std::string& file) {
    FILE *fp = nullptr;
    BOOST_SCOPE_EXIT_ALL(&) {
        if (fp) fclose(fp);
    };


    //
    // open file for writing
    //
    fp = fopen(file.c_str(), "wb");
    if (!fp)
        throw std::runtime_error("Cannot open file.");


    //
    // create necessary structs
    //
    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    BOOST_SCOPE_EXIT_ALL(&) {
        if (png_ptr) png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    };

    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr)
        throw std::runtime_error("Cannot create png_struct.");

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
        throw std::runtime_error("Cannot create png_info.");


    //
    // set up error & warning handler
    //

    // the default error handler is unsafe in C++ because it uses setjmp & longjmp to handle errors
    png_set_error_fn(
        png_ptr,
        nullptr,
        user_error_fn, /* if null, use default error handler with setjmp & longjmp!!! */
        user_warning_fn /* if null, use default warning handler */);


    //
    // init
    //
    png_init_io(png_ptr, fp);

    // set write callback
    png_set_write_status_fn(
        png_ptr,
        write_row_callback /* will be called when a row is written */);

    // set compress level
    png_set_compression_level(png_ptr, Z_BEST_COMPRESSION);


    //
    // write header
    //
    png_uint_32 width = 0;
    png_uint_32 height = 0;
    int bit_depth = 0;
    int color_type = 0;
    int interlace_type = 0;
    int compression_type = 0;
    int filter_method = 0;
    png_set_IHDR(
        png_ptr,
        info_ptr,
        width,
        height,
        bit_depth,
        color_type,
        interlace_type,
        compression_type,
        filter_method);

    png_write_info(
        png_ptr,
        info_ptr);


    //
    // write image
    //

    // write a whole image
    std::vector<png_bytep> row_pointers(height);
    png_write_image(png_ptr, row_pointers.data());

    // write several rows
    int number_of_rows = 1;
    png_write_rows(png_ptr, row_pointers.data(), number_of_rows);

    // write a single row
    png_bytep row_pointer = nullptr;
    png_write_row(png_ptr, row_pointer);

    // finish writing
    png_write_end(png_ptr, nullptr);
}


int main() {
    try {
        read_png("test.png");
        write_png("test.png");
    }
    catch (const std::exception& e) {
        std::cout << "exception : " << e.what() << "\n";
    }
}
