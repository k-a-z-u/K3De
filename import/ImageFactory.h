#ifndef IMAGEFACTORY_H
#define IMAGEFACTORY_H


#include "Image.h"
#include <string>
#include <png.h>
#include <jpeglib.h>

class ImageFactory {

public:

	static Image load(const std::string& filename) {
		std::string ext = filename.substr(filename.length() - 3, 3);
		if (ext == "png") {return loadPNG(filename);}
		if (ext == "jpg") {return loadJPG(filename);}
		throw "unsupported format";
	}

private:

	static Image loadJPG(const std::string& filename) {

		Image img;
		FILE* fp = fopen(filename.c_str(), "rb");  //open the file

		unsigned char* rowptr[1];
		unsigned char* jdata;
		struct jpeg_decompress_struct info;
		struct jpeg_error_mgr err;

		info.err = jpeg_std_error(&err);
		jpeg_create_decompress(& info);   //fills info structure

		//if the jpeg file doesn't load
		if(!fp) {throw "error reading JPEG";}

		jpeg_stdio_src(&info, fp);
		jpeg_read_header(&info, TRUE);   // read jpeg file header

		jpeg_start_decompress(&info);    // decompress the file

		//set width and height
		img.w = info.output_width;
		img.h = info.output_height;
		const int channels = info.num_components;

		switch (channels) {
			case 1: img.format = ImageFormat::IMAGE_GREY; break;
			case 3: img.format = ImageFormat::IMAGE_RGB; break;
			case 4: img.format = ImageFormat::IMAGE_RGBA; break;
			default: throw "unsupported format";
		}

		img.data.resize(img.w*img.h*channels);

		//--------------------------------------------
		// read scanlines one at a time & put bytes
		//    in jdata[] array. Assumes an RGB image
		//--------------------------------------------
		jdata = img.data.data();
		while (info.output_scanline < info.output_height) {

			// Enable jpeg_read_scanlines() to fill our jdata array
			rowptr[0] = (unsigned char *)jdata +  // secret to method
					channels * info.output_width * info.output_scanline;

			jpeg_read_scanlines(&info, rowptr, 1);
		}
		//---------------------------------------------------

		jpeg_finish_decompress(&info);   //finish decompressing

		//----- create OpenGL tex map (omit if not needed) -------

		fclose(fp);                    //close the file

		return img;

	}

	static Image loadPNG(const std::string& filename) {

		Image img;
		FILE* fp = fopen(filename.c_str(), "rb");

		png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		if (!png) {throw "Could not allocate read struct\n";}

		png_infop info_ptr = png_create_info_struct(png);
		if (!info_ptr) {throw "error 2";}

		if (setjmp(png_jmpbuf(png))) {throw "error 3";}

		// custom read callback function. starts to read at data and will increment the ptr with every read. thus: &data
		//uint8_t* data = src.getData();
		//png_set_read_fn(png, (void*) &data, PngReadCallback);
		png_init_io(png, fp);

		// read the details of the png
		png_read_info(png, info_ptr);
		const int w = png_get_image_width(png, info_ptr);
		const int h = png_get_image_height(png, info_ptr);
		const int c = png_get_channels(png, info_ptr);
		const int bits = png_get_bit_depth(png, info_ptr);

		// adjust target
		if		(c == 1 && bits == 8)	{img.format = ImageFormat::IMAGE_GREY;}
		else if (c == 2 && bits == 8)	{img.format = ImageFormat::IMAGE_GREY_ALPHA;}
		else if	(c == 3 && bits == 8)	{img.format = ImageFormat::IMAGE_RGB;}
		else if (c == 4 && bits == 8)	{img.format = ImageFormat::IMAGE_RGBA;}
		else							{throw "unsupported format";}

		// resize target
		img.w = w;
		img.h = h;
		img.data.resize(w*h*c*bits/8);

		// create array of row-pointers
		uint8_t* rows[h];
		for (int y = 0; y < h; ++y) {
			rows[y] = &img.data[y*w*c*bits/8];
//			switch(img.format) {
//				case ImageFormat::IMAGE_GREY:		rows[y] = &img.data[y*w*1]; break;
//				case ImageFormat::IMAGE_GREY_ALPHA:	rows[y] = &img.data[y*w*2]; break;
//				case ImageFormat::IMAGE_RGB:		rows[y] = &img.data[y*w*3]; break;
//				case ImageFormat::IMAGE_RGBA:		rows[y] = &img.data[y*w*4]; break;
//			}
		}

		// read
		png_read_image(png, rows);

		// cleanup
		png_destroy_read_struct(&png, &info_ptr, nullptr);

		return img;

	}

};

#endif // IMAGEFACTORY_H
