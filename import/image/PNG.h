#ifndef K3DE_PNG_H
#define K3DE_PNG_H

#include "../Image.h"
#include "../../data/Data.h"
#include "../../Exception.h"

#ifdef WITH_PNG
#include <png.h>
#endif

namespace K3De {

	class PNG {

	public:

		#ifdef WITH_PNG
		static void PngReadCallback(png_structp pngPtr, png_bytep data, png_size_t length) {
			png_voidp a = png_get_io_ptr(pngPtr);
			uint8_t** src = (uint8_t**) a;
			memcpy(data, *src, length);
			*src += length;
		}
		#endif

		static Image load(const Data& d) {

		#ifdef WITH_PNG

			png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
			if (!png) {throw "Could not allocate read struct\n";}

			png_infop info_ptr = png_create_info_struct(png);
			if (!info_ptr) {throw "error 2";}

			if (setjmp(png_jmpbuf(png))) {throw "error 3";}

			// custom read callback function. starts to read at data and will increment the ptr with every read. thus: &data
			const uint8_t* data = d.get();
			png_set_read_fn(png, (void*) &data, PngReadCallback);
			//png_init_io(png, fp);

			// read the details of the png
			png_read_info(png, info_ptr);
			const int w = png_get_image_width(png, info_ptr);
			const int h = png_get_image_height(png, info_ptr);
			const int c = png_get_channels(png, info_ptr);
			const int bits = png_get_bit_depth(png, info_ptr);

			if (bits != 8) {throw Exception("only 8-bit PNGs are supported");}

			// sanity check
			if (png_get_color_type(png, info_ptr) == PNG_COLOR_TYPE_PALETTE) {
				throw Exception("indexed PNGs are not supported");
				// use imagemagick to convert:
				// convert icon_flag_blue.png -strip png32:icon_flag_blue2.png
			}

			// adjust target
			ImageFormat format;
			if		(c == 1 && bits == 8)	{format = ImageFormat::IMAGE_GREY;}
			else if (c == 2 && bits == 8)	{format = ImageFormat::IMAGE_GREY_ALPHA;}
			else if	(c == 3 && bits == 8)	{format = ImageFormat::IMAGE_RGB;}
			else if (c == 4 && bits == 8)	{format = ImageFormat::IMAGE_RGBA;}
			else							{throw Exception("unsupported format");}

			// resize target
			Image img(w, h, format);

			// create array of row-pointers
			uint8_t* rows[h];
			for (int y = 0; y < h; ++y) {
				rows[y] = &img.getDataPtr()[y*w*c*bits/8];
			}

			// read
			png_read_image(png, rows);

			// cleanup
			png_destroy_read_struct(&png, &info_ptr, nullptr);

			return img;

		#else
			(void) d;
			throw Exception("not compiled with PNG support. used -DWITH_PNG");
		#endif

		}

		static void save(const std::string& file, const Image& img) {

		#ifdef WITH_PNG

			png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
			if (!png) {throw "Could not allocate write struct\n";}

			png_infop info_ptr = png_create_info_struct(png);
			if (!info_ptr) {throw "error 2";}

			if (setjmp(png_jmpbuf(png))) {throw "error 3";}

			const int w = img.getWidth();
			const int h = img.getHeight();

			// configure
			int components = 0;
			switch(img.getFormat()) {
				case ImageFormat::IMAGE_GREY:	components = 1; png_set_IHDR(png, info_ptr, w, h, 8, PNG_COLOR_TYPE_GRAY, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT); break;
				case ImageFormat::IMAGE_RGB:	components = 3; png_set_IHDR(png, info_ptr, w, h, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT); break;
				case ImageFormat::IMAGE_RGBA:	components = 4; png_set_IHDR(png, info_ptr, w, h, 8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT); break;
				default: throw Exception("unsupported format");
			}

			// create an array containing the start of each row within the image
			uint8_t* rows[h];
			for (int y = 0; y < h; ++y) {
				rows[y] = (uint8_t*) &(img.getData()[y*w*components]);
			}
			png_set_rows(png, info_ptr, &rows[0]);

			// write
			FILE* fp = fopen(file.c_str(), "wb");
			png_init_io (png, fp);
			png_write_png(png, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
			fclose(fp);

			// cleanup
			png_destroy_write_struct (&png, &info_ptr);

		#else
			(void) d;
			throw Exception("not compiled with PNG support. used -DWITH_PNG");
		#endif

		}

	};

}

#endif // K3DE_PNG_H
