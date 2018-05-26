#ifndef K3DE_JPG_H
#define K3DE_JPG_H

#include "../Image.h"
#include "../../data/Data.h"
#include "../../Exception.h"

#ifdef WITH_JPEG
#include <jpeglib.h>
#endif

namespace K3De {

	class JPG {

	public:

		static Image load(const Data& d) {

		#ifdef WITH_JPEG



			unsigned char* rowptr[1];
			unsigned char* jdata;
			struct jpeg_decompress_struct info;
			struct jpeg_error_mgr err;

			info.err = jpeg_std_error(&err);
			jpeg_create_decompress(& info);   //fills info structure

			jpeg_mem_src(&info, d.get(), d.size());
			jpeg_read_header(&info, TRUE);   // read jpeg file header

			jpeg_start_decompress(&info);    // decompress the file

			const int channels = info.num_components;
			ImageFormat format;
			switch (channels) {
				case 1: format = ImageFormat::IMAGE_GREY; break;
				case 3: format = ImageFormat::IMAGE_RGB; break;
				case 4: format = ImageFormat::IMAGE_RGBA; break;
				default: throw "unsupported format";
			}

			Image img(info.output_width, info.output_height, format);

			//--------------------------------------------
			// read scanlines one at a time & put bytes
			//    in jdata[] array. Assumes an RGB image
			//--------------------------------------------
			jdata = img.getDataPtr();
			while (info.output_scanline < info.output_height) {

				// Enable jpeg_read_scanlines() to fill our jdata array
				rowptr[0] = (unsigned char *)jdata +  // secret to method
						channels * info.output_width * info.output_scanline;

				jpeg_read_scanlines(&info, rowptr, 1);
			}
			//---------------------------------------------------

			jpeg_finish_decompress(&info);   //finish decompressing

			//----- create OpenGL tex map (omit if not needed) -------

			return img;

		#else

			(void) d;
			throw Exception("not compiled with JPEG support. used -DWITH_JPEG");

		#endif

		}

		static void save(const std::string& file, const Image& img) {

		#ifdef WITH_JPEG

			struct jpeg_compress_struct cinfo;
			struct jpeg_error_mgr jerr;

			JSAMPROW row_pointer[1];

			FILE* fp = fopen(file.c_str(), "wb");
			if (!fp) {throw Exception("error while saving JPG: " + file);}

			cinfo.err = jpeg_std_error( &jerr );
			jpeg_create_compress(&cinfo);
			jpeg_stdio_dest(&cinfo, fp);

			cinfo.image_width = img.getWidth();
			cinfo.image_height = img.getHeight();

			switch (img.getFormat()) {
				case ImageFormat::IMAGE_GREY:	cinfo.input_components = 1;	cinfo.in_color_space = JCS_GRAYSCALE; break;
				case ImageFormat::IMAGE_RGB:	cinfo.input_components = 3;	cinfo.in_color_space = JCS_RGB; break;
				default: throw "unsupported format";
			}

			jpeg_set_defaults( &cinfo );
			jpeg_start_compress( &cinfo, TRUE );

			while( cinfo.next_scanline < cinfo.image_height ) {
				uint8_t* ptr = (uint8_t*) &img.getDataPtr()[ cinfo.next_scanline * cinfo.image_width * cinfo.input_components];
				row_pointer[0] = ptr;
				jpeg_write_scanlines( &cinfo, row_pointer, 1 );
			}

			jpeg_finish_compress(&cinfo);
			jpeg_destroy_compress(&cinfo);
			fclose(fp);

		#else

			(void) file;
			(void) img;
			throw Exception("not compiled with JPEG support. used -DWITH_JPEG");

		#endif


		}


	};

}

#endif // K3DE_JPG_H
