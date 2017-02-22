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
			throw Exception("not compiled with JPEG support. used -DWITH_JPEG");

		#endif

		}


	};

}

#endif // K3DE_PNG_H
