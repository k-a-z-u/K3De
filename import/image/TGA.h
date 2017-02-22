#ifndef K3DE_TGA_H
#define K3DE_TGA_H

#include "../Image.h"
#include "../../data/Resource.h"
#include "../../data/ResourceFactory.h"

// http://paulbourke.net/dataformats/tga/

namespace K3De {

	class TGA {

		/** 1 byte image type */
		enum ImageType : uint8_t {
			NO_IMAGE_DATA = 0,
			UNCOMPRESSED_COLOR_MAPPED = 1,
			UNCOMPRESSED_TRUE_COLOR = 2,
			UNCOMPRESSED_BW = 3,
			RLE_COLOR_MAPPED = 9,
			RLE_TRUE_COLOR = 10,
			RLE_BW = 11,
		};

#pragma pack(push,1)
		struct ColorMap {
			uint16_t idx1;
			uint16_t numEntries;
			uint8_t bitsPerPixel;
		};// __attribute__((__packed__));


		struct ImageSpecs {

			uint16_t originX;
			uint16_t originY;

			uint16_t width;
			uint16_t height;

			uint8_t bitsPerPixel;
			uint8_t imageDesc;			// bits 3-0 give the alpha channel depth, bits 5-4 give direction

		};// __attribute__((__packed__));

		struct Header {

			uint8_t imageIDLength;
			uint8_t colorMapType;
			ImageType imageType;
			ColorMap colorMap;
			ImageSpecs specs;


		};// __attribute__((__packed__));
#pragma pack(pop)


	public:


		static void save(const std::string& file, const Image& _img) {

			// upside down. needed for TGA
			Image img = _img.invertedY();

			Header h;
			memset(&h, 0, sizeof(h));

			//h.imageIDLength = 0;
			//h.colorMapType = 0;
			h.imageType = UNCOMPRESSED_TRUE_COLOR;
			//h.specs.originX = 0;
			//h.specs.originY = 0;
			h.specs.width = img.getWidth();
			h.specs.height = img.getHeight();

			std::vector<uint8_t> copy = img.getData();

			switch(img.getFormat()) {

				case ImageFormat::IMAGE_RGB:
					h.specs.bitsPerPixel = 24;
					for (size_t i = 0; i < copy.size(); i += 3) {
						std::swap(copy[i+0], copy[i+2]);		// from BGR to RGB
					}
					break;

				case ImageFormat::IMAGE_RGBA:
					h.specs.bitsPerPixel = 32;											// 32 bit
					h.specs.imageDesc = 8;												// using 8 bit alpha
					for (size_t i = 0; i < copy.size(); i += 4) {
						std::swap(copy[i+0], copy[i+2]);		// from BGRA to RGBA
					}
					break;

				default: throw Exception("unsupported output format");

			}

			std::ofstream fs(file, std::ios::binary);
			fs.write((char*)&h, sizeof(h));
			fs.write((char*)copy.data(), copy.size());
			fs.close();

		}

		static Image load(const Data& d) {

			// cast data to header
			Header* header = (Header*) d.get();

			// sanity checks
			if (header->specs.originX != 0)						{throw Exception("x-origin not yet supported");}
			if (header->specs.originY != 0)						{throw Exception("y-origin not yet supported");}
			if (header->colorMap.numEntries != 0)				{throw Exception("color-map must be empty!");}

			if ((header->imageType != UNCOMPRESSED_TRUE_COLOR) && (header->imageType != UNCOMPRESSED_BW)) {
				throw Exception("currently only uncompressed true-color and BW images are supported");
			}

			if (header->specs.bitsPerPixel ==  8 && header->specs.imageDesc != 0)	{throw Exception("Image Descriptor must currently be zero!");}
			if (header->specs.bitsPerPixel == 24 && header->specs.imageDesc != 0)	{throw Exception("Image Descriptor must currently be zero!");}
			if (header->specs.bitsPerPixel == 32 && header->specs.imageDesc != 8)	{throw Exception("Alpha Channel must be 8 bits");}

			// after the header follows the image-ID-Field and the [empty] palette, then -> data!
			const uint8_t* rawStart = d.get() + (sizeof(Header) + header->imageIDLength + 0);		// why +1?

			Image img;

			// 8-Bit gray
			if ( (header->imageType == UNCOMPRESSED_BW) && (header->specs.bitsPerPixel == 8) ) {

				img = Image(header->specs.width, header->specs.height, ImageFormat::IMAGE_GREY);
				uint8_t* imgData = img.getDataPtr();
				memcpy(imgData, rawStart, img.getWidth()*img.getHeight()*1);

			// 24-Bit RGB
			} else if ( (header->imageType == UNCOMPRESSED_TRUE_COLOR) && (header->specs.bitsPerPixel == 24) ) {

				img = Image(header->specs.width, header->specs.height, ImageFormat::IMAGE_RGB);
				uint8_t* imgData = img.getDataPtr();
				memcpy(imgData, rawStart, img.getWidth()*img.getHeight()*3);

				for (size_t i = 0; i < img.getData().size(); i += 3) {
					std::swap(imgData[i+0], imgData[i+2]);		// from BGR to RGB
				}

			// 32-Bit RGBA
			} else if ( (header->imageType == UNCOMPRESSED_TRUE_COLOR) && (header->specs.bitsPerPixel == 32) ) {

				img = Image(header->specs.width, header->specs.height, ImageFormat::IMAGE_RGBA);
				uint8_t* imgData = img.getDataPtr();
				memcpy(imgData, rawStart, img.getWidth()*img.getHeight()*4);

				for (size_t i = 0; i < img.getData().size(); i += 4) {
					std::swap(imgData[i+0], imgData[i+2]);		// from BGRA to RGBA
				}

			} else {
				throw Exception("unsupported bit depth: " + std::to_string(header->specs.bitsPerPixel));
			}

			// upside down. needed for TGA
			img = img.invertedY();

			return img;

		}


	};

}

#endif // K3DE_TGA_H
